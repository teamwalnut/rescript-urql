# reason-urql

Reason bindings for Formidable's Universal React Query Library (`urql`) https://github.com/FormidableLabs/urql

> Warning: These bindings are in an experimental state. Do not use in production (yet) 😉.

## Table of Contents

- [What is `reason-urql`?](#what-is-reason-urql?)
- [Run the Example Project](#run-the-example-project)
- [Installation](#installation)
- [API](#api)
  - [`Query`](#query)
    - [`Query` API](#query-api)
  - [`Mutation`](#mutation)
    - [`Mutation` API](#mutation-api)
  - [`Client`](#client)
    - [`Client` API](#client-api)
    - [Custom caches](#custom-caches)
  - [`Provider`](#provider)
    - [`Provider` `props`](#provider-props)
  - [`Connect`](#connect)
    - [Pattern Matching with `response`](#pattern-matching-with-response)
    - [Mutations and `Connect`](#mutations-and-connect)
    - [`Connect` `props`](#connect-props)
      - [`Connect`'s `render` `prop`](#connects-render-prop)
- [Getting Involved](#getting-involved)

## What is `reason-urql`?

`urql` is a GraphQL client for React, allowing you to hook up your components to queries, mutations, and subscriptions. `reason-urql` provides bindings to `urql` that allow you to use the API in Reason. We are working hard to support the full API – check back regularly for updates.

## Run the Example Project

The example project is a simple app for viewing and liking Formidable dogs. To get the example running locally:

```sh
# in one terminal, compile the app in watch mode
yarn start

# in another terminal, start webpack for the app
yarn start-demo-app

# open up the app either by clicking the index.html file in your file browser
# or calling it from the command line
open example/app/index.html
```

You should now be able to edit the example freely, and refresh the page to see changes take effect. We're working on getting hot reloading into the example soon!

## Installation

These bindings have not yet been published to `npm`. Check back soon for updates! In the meantime, you can always clone locally and play with the example.

One important thing to note: this project uses `graphql_ppx` to type check your GraphQL queries and mutations. You'll need to add it as a dev dependency.

```sh
yarn add graphql_ppx --dev
```

And set it in your `ppx_flags` in `bsconfig.json`.

```json
{
  "ppx-flags": ["graphql_ppx/ppx"]
}
```

Finally, you'll need to send an introspection query to your GraphQl API. This allows `graphql_ppx` to generate a `graphql_schema.json` at the root of your project that it can use to type check your queries. To do this:

```sh
yarn send-introspection-query https://mygraphqlapi.org/graphql
```

See the [docs for `graphql_ppx`](https://github.com/mhallin/graphql_ppx) for more assistance.

## API

### Query

**Before reading this section, read the docs on `urql`'s [`Query` API](https://github.com/FormidableLabs/urql#query).**

The `Query` `module` provides the core building block for genertaing queries in `reason-urql`. We use the power of [`graphql_ppx`](https://github.com/mhallin/graphql_ppx) to support typed GraphQL queries that will validate against your schema _at compile time_. 🎉

To generate a query, simply define your GraphQL query module and pass its `make` function to `reason-urql`s `query` function. This will return an `urql` query object.

```reason
open ReasonUrql;

module GetAllDogs = [%graphql
  {|
query dogs {
  dogs {
    key
    name
    breed
  }
}|}
];

let myQuery = Query.query(GetAllDogs.make());
```

Adding variables to your queries is as simple as passing named arguments to your query `module`'s `make` function.

```reason
open ReasonUrql;

module GetDog = [%graphql
  {|
query dog($key: ID!) {
  dog(key: $key) {
    key
    name
    breed
  }
}|}
];

let myQuery = Query.query(GetDog.make(~key="VmeRTX7j-", ()));
```

In addition, there may be situations where you want to make a query but don't know the value of your variables ahead of time; this can be true if you want to pass queries around your application to different `Connect` components. To support this use case, we supply a [functor](https://reasonml.github.io/docs/en/module#module-functions-functors) in the `Query` `module`, simply called `Make`. It accepts a query `module` and returns a new `module` with a curried function called `queryFn`. `queryFn` has your GraphQL query applied to it and accepts a `variables` argument, of type `Js.Json.t`; it returns an `urql` query object. Here's how you'd use it in action.

```reason
open ReasonUrql;

module GetDog = [%graphql
  {|
query dog($key: ID!) {
  dog(key: $key) {
    key
    name
    breed
  }
}|}
];

module GetDogQuery = Query.Make(GetDog);

let myQuery = GetDogQuery.queryFn(
  ~variables=Json.Encode.(object_([("key", string("VmeRTX7j-"))])),
  (),
);
```

You can also access the standard `urql` query object, with no variables applied, by accessing `query` on your new module:

```reason
open ReasonUrql;

module GetAllDogs = [%graphql
  {|
query dogs {
  dogs {
    key
    name
    breed
  }
}|}
];

module GetAllDogsQuery = Query.Make(GetAllDogs);

let myQuery = GetAllDogsQuery.query;
```

#### `Query` API

**`query`**

**type** – `({. "parse": Js.Json.t => 'a, "query": string, "variables": Js.Json.t}) => urqlQuery`. This is equivalent to: `(GraphQLQuery.make()) => urqlQuery`.

**description** – a function for generating an `urql` query object, given an instance of a GraphQL query.

**`Make`**

**type** – `module type { let query: string } => module type { let query: urqlQuery; let queryFn: (~variables: option(Js.Json.t)) => urqlQuery; }`

**description** – a functor for generating a `module` with your GraphQL query. Provides both `queryFn`, a curried function for generating an `urql` query object with `variables`, and `query`, the `urql` query object with no `variables` applied.

### Mutation

**Before reading this section, read the docs on `urql`'s [`Mutation` API](https://github.com/FormidableLabs/urql#mutation).**

The bindings for `reason-urql`'s `mutation` API is very similar to that of `query` and also uses `graphql_ppx` under the hood.

To generate a mutation, simply write your GraphQL mutation and call `reason-urql`s `mutation` function.

```reason
open ReasonUrql;

module LikeDog = [%graphql
  {|
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      key
      name
      breed
      likes
    }
  }
|}
];

let mutation = Mutation.mutation(LikeDog.make(~key="VmeRTX7j-", ()));
```

Just like the `Query` `module`, we also provide a functor called `Make` to support situations where you may not know the values of your variables when you need to construct your mutation. `Make` returns both `mutationFn`, a curried function that expects a `variables` argument, and `mutation`, the `urql` mutation object with no `variables` applied.

```reason
open ReasonUrql;

module LikeDog = [%graphql
  {|
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      key
      name
      breed
      likes
    }
  }
|}
];

module LikeDogMutation = Mutation.Make(LikeDog);

let myMutation = LikeDogMutation.mutationFn(
  ~variables=Json.Encode.(object_([("key", string("VmeRTX7j-"))])),
  (),
);
```

This comes in very handy when wiring multiple mutations up to `Connect` where you want to pass the partially applied (curried) function as a callback to child components. See the [Mutations and `Connect`](#mutations-and-connect) section of this guide for more details.

#### `Mutation` API

**`mutation`**

**type** – `({. "parse": Js.Json.t => 'a, "query": string, "variables": Js.Json.t}) => urqlMutation`. This is equivalent to: `(GraphQLMutation.make()) => urqlMutation`.

**description** – a function for generating an `urql` mutation object, given an instance of a GraphQL mutation.

**`Make`**

**type** – `module type { let query: string } => module type { let mutation: urqlMutation; let mutationFn: (~variables: option(Js.Json.t)) => urqlMutation; }`

**description** – a functor for generating a `module` with your GraphQL mutation. Provides both `mutationFn`, a curried function for generating an `urql` mutation object with `variables`, and `mutation`, the `urql` mutation object with no `variables` applied.

### `Client`

**Before reading this section, read the docs on `urql`'s [`Client` API](https://github.com/FormidableLabs/urql#client).**

`urql`'s `Client` API takes a config object containing values for `url`, `cache`, `initialCache`, and `fetchOptions`. We model this config as a `[@bs.deriving abstract]`, BuckleScript's [implementation for JavaScript objects](https://bucklescript.github.io/docs/en/object#record-mode). To create a new `Client` using `reason-urql`, simply call the `make` function from the `Client` module:

```reason
open ReasonUrql;

let client = Client.make(~url="https://myapi.com/graphql", ());
```

In order to pass `fetchOptions` to your `Client`, you'll need to create them using the `Fetch.RequestInit.make()` function from [`bs-fetch`](https://github.com/reasonml-community/bs-fetch). Using this function guarantees that the options you are passing to `urql`'s `fetch` calls are valid and type safe. To set this up with `reason-urql`, do something like the following:

```reason
open ReasonUrql;

let makeFetchOptions =
  Fetch.RequestInit.make(
    ~method_=Post,
    ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
    (),
  );

/* Wrap your fetchOptions in the fetchOptions variant, which accepts the Cient.FetchObj or Client.FetchFn constructor. */
let fetchOptions = Client.FetchObj(makeFetchOptions);

let client = Client.make(~url="http://localhost:3001", ~fetchOptions, ());
```

In `urql`, your `fetchOptions` argument can either be an object or a function returning an object: `RequestInit | () => RequestInit`. We use variants to model this in `reason-urql`.

```reason
type fetchOptions =
  | FetchObj(Fetch.requestInit)
  | FetchFn(unit => Fetch.requestInit);
```

Once the `Client` is instantiated, you get access to its methods `executeQuery` and `executeMutation`. Since these APIs are `Promise`-based on the JS side of things, you'll need to use [Reason's `Promise` syntax](https://reasonml.github.io/docs/en/promise) to use them. For example:

```reason
open ReasonUrql;

module GetAllDogs = [%graphql
  {|
query dogs {
  dogs {
    name
    breed
    description
  }
}|}
];
let queryAllDogs = Query.query(GetAllDogs.make());

let client = Client.make(~url="http://localhost:3001", ());

Client.executeQuery(~client, ~query=queryAllDogs, ~skipCache=false)
|> Js.Promise.then_(value => {
     let dogs = value##data##dogs;
     Js.log2("Dogs", dogs);
     Js.Promise.resolve(dogs);
   })
|> Js.Promise.catch(err => {
     Js.log2("Something went wrong!", err);
     Js.Promise.resolve(err);
   });
```

#### Custom caches

**Before reading this section, read the docs on [custom caches](https://github.com/FormidableLabs/urql#custom-caches) with `urql`.**

The `cache` parameter on `Client.make` allows you to create and manage your own cache (if you're into that sort of thing). To create a custom cache, you must provide a Reason record of the following shape:

```
type cache('queryData, 'store) = {
  write,
  read: read('queryData),
  invalidate,
  invalidateAll,
  update: update('queryData, 'store),
};
```

What are all these parameters?! `urql`'s caching mechanism is Promise-based and requires you to implement each of these methods to manage your cache. These methods have the following signatures:

**`write`** – `(~hash: string, ~data: data) => Js.Promise.t(unit);`
Given a hash and some data (the result of executing a query or mutation), write your data to the cache. `data` is given an abstract type, so you won't be able to access its internal structure. But you shouldn't have to – think of `write` as being purely responsible for determining how your query data arrives in the cache.

**`read`** - `(~hash: string) => Js.Promise.t('queryData);`
Given a hash, resolve the data associated with that hash from the cache. `read` accepts a type parameter, `'queryData`, corresponding to the shape of `data` being stored in the cache. We recommend allowing Reason's excellent type inference to provide the `'queryData` type for you and alerting you to ways in which you're accessing it unsafely.

**`invalidate`** - `(~hash: string) => Js.Promise.t(unit);`
Given a hash, invalidate the data associated with that hash in the cache. Typically, this will mean removing the entry entirely from your cache.

**`invalidateAll`** – `unit => Js.Promise.t(unit);`
Invalidate all entries in your `cache`. Typically, this means clearing out the cache entirely.

**`update`** – `(~callback: ('store, string, 'queryData) => unit) => Js.Promise.t(unit);`
Execute a provided `callback` function on every entry in the `cache`. `callback` should be a function that accepts your `cache` (which is typed using the type parameter `store`), a hash of type `string`, and the data associated with that `hash` of type `'queryData`.

All of these functions, except for `write`, will be passed to any `Connect`ed component (see below) throughout the application. You can then invoke them anywhere to manage your cache. Custom caching is an advanced feature of `urql` and isn't necessary to create a performant application. Use with discretion.

#### `Client` API

**`make`** – create an `urqlClient`.

**type** – `(~url: string, cache: option(cache('queryData, 'store))=?, initialCache=option('store)=?, fetchOptions=fetchOptions?, unit) => client`

**description** – creates an instance of an `urql` `client`, which may be passed to `Provider`. `make` accepts 1 required argument, `url`, and 3 optional arguments, `cache`, `initialCache`, and `fetchOptions`.

`url` – `string`, required. Your GraphQL endpoint.

`cache` – `cache('queryData, 'store)`, optional. A Reason record with keys for `write`, `read`, `invalidate`, `invalidateAll`, and `update`. Allows management of your own cache. `urql`'s default cache will be used if no cache is provided. Takes two type parameters, `'queryData` for typing the structure of data stored in the cache, and `'store`, for typing the structure of the cache itself.

`initialCache` – `'store`, optional. The data structure for storing cache data. If none is provided, `urql`'s default cache of type `Js.t({..})` will be used.

`fetchOptions` – `FetchObj(Fetch.requestInit) | FetchFn(unit => Fetch.requestInit)`, optional. A variant constructor that accepts either an instance of `Fetch.requestInit` or a function returning an instance of `Fetch.requestInit`. To see valid options for `Fetch.requestInit`, check out [`bs-fetch`](https://github.com/reasonml-community/bs-fetch).

**`executeQuery`**

**type** – `(~client: client, ~query: Query.urqlQuery, ~skipCache: bool) => Js.Promise.t('a)`.

**description** – execute a GraphQL query given an `urql` query object.

**`executeMutation`**

**type** – `(~client: client, ~mutation: Mutation.urqlMutation) => Js.Promise.t('a)`.

**description** – execute a GraphQL mutation given an `urql` mutation object.

### Provider

**Before reading this section, read the docs on `urql`'s [`Provider` API](https://github.com/FormidableLabs/urql#provider).**

To support the `Provider` component in ReasonReact, we take advantage of ReasonReact's excellent [`wrapJSForReason` helper](https://reasonml.github.io/reason-react/docs/en/interop#reasonreact-using-reactjs). `Provider` accepts a single `prop`, `client`. `client` is an instance of an `urql` `client` (see previous section). For example:

```reason
open ReasonUrql;

/* After instantiating our client (see above), we can wrap our app in the `Provider` component */
let component = ReasonReact.statelessComponent("App");

let client = Client.make(~url="http://localhost:3001", ());

let make = _children => {
  ...component,
  render: _self => <Provider client> <Header /> <Layout /> </Provider>,
};
```

#### `Provider` `props`

**`client`**

**type** – `Client.client`.

**description** – an instance of an `urql` `client`. To create a client, simply call the `Client.make` function.

### Connect

**Before reading this section, read the docs on `urql`'s [`Connect` API](https://github.com/FormidableLabs/urql#connect).**

Once you've wrapped your app in the `Provider` component, you can use `urql`'s `Connect` component to wire up UI to queries, mutations, and your cache. `Connect` uses the [render prop](https://reactjs.org/docs/render-props.html) pattern.

While `urql` names its render prop `children`, we opt to name it `render` on the Reason side because `children` is a reserved keyword for ReasonReact components – naming a prop `children` will result in compiler errors. `Connect` provides a `Js.t` object to `render`, which contains a set of known fields and a set of user-supplied mutations, provided in the `mutation` prop. We use a simple `Js.t` object and `Js.Obj.assign` to grab the user-supplied mutations from `urql`.

```reason
/* Types for the object supplied to `render`. User-supplied mutations will be spread into this object and available as functions. */
type renderArgs('data, 'queryData, 'store) = {
  .
  "response": response('data),
  "fetching": bool,
  "loaded": bool,
  "data": option('data),
  "error": option(error),
  "refetch": refetch,
  "refreshAllFromCache": refreshAllFromCache,
  "cache": cache('queryData, 'store),
};
```

The render prop accepts a type argument `('data)` to type the data returned from your GraphQL API. With our `graphql_ppx` integration, this is as simple as grabbing the `t` type off of your query module. Let's look at an example:

```reason
open ReasonUrql;

/* Define a GraphQL query using graphql_ppx module. */
module GetAllDogs = [%graphql
  {|
query dogs {
  dogs {
    key
    name
    breed
    description
    imageUrl
    likes
  }
}|}
];

/* Format the query for urql. */
let query = Query.query(GetAllDogs.make());

/* Pass the type from the GetAllDogs module to Connect.renderArgs. This will properly type your data. */
let make = (_children) => {
  ...component,
  render: _ => <Connect
    query
    render={(result: Connect.renderArgs(GetAllDogs.t, 'queryData, 'store)) =>
      switch (result##response) {
      | Loading => <Loading />
      | Data(data) =>
        <div>
          {
            Array.map(
              dog =>
                switch (dog) {
                | Some(dog) =>
                  <Dog
                    key=dog##key
                    description=dog##description
                    id=dog##key
                    imageUrl=dog##imageUrl
                    name=dog##name
                    likes=dog##likes
                  />
                | None => <div />
                },
              data##dogs,
            )
            |> ReasonReact.array
          }
        </div>
      | Error(error) => <Error />
      }
    }
  />
}
```

Awesome! We get the power of `Connect`'s render prop convention to connect our UI to a GraphQL query, all while maintaining type saftey with Reason 🚀.

#### Pattern Matching with `response`

You may have noticed in the above example that `result##response` returns a variant containing three constructors, `Loading`, `Data(data)`, and `Error(error)`. This makes rendering conditional UI a breeze in ReasonReact. Each constructor will be passed the proper payload, whether that is `data` matching the user-supplied `'data` type or `error` of the form `{. "message": string }`.

#### Mutations and `Connect`

`urql` does a pretty nifty thing to support mutations on the `Connect` component. It takes a user-supplied `mutation` map and turns each mutation into a function accessible on the object passed to `Connect`'s render prop. It accomplishes this through props spreading, [a technique that Reason does not support](https://reasonml.github.io/reason-react/docs/en/props-spread). Props spreading can be modeled easily on the TypeScript side using intersection types. It gets a bit trickier to model this behavior on the Reason end while still maintaining robust type safety. We can't intersect a set of known types (the render prop argument) with unknown, user-supplied types (mutations) behind the scenes – only the user can know what they need and how it's typed.

To alleviate some of this difficulty, we use BuckleScript's [`JS.Dict` API](https://bucklescript.github.io/docs/en/object#hash-map-mode) to model the `mutation` prop.

```reason
open ReasonUrql;

type mutationMap = Js.Dict.t(Mutation.urqlMutation);
```

To set up a mutation map, you can do the following:

```reason
open ReasonUrql;

/* Define a GraphQL mutation. */
module LikeDog = [%graphql
  {|
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      name
      key
      breed
      likes
    }
  }
|}
];

module LikeDogMutation = Mutation.Make(LikeDog);

let mutationMap: Connect.mutationMap = Js.Dict.empty();
Js.Dict.set(mutationMap, "likeDog", LikeDogMutation.mutation);

let mutationMap = Js.Dict.empty();
Js.Dict.set(mutationMap, "likeDog", likeDog);

/* On your Connect component, pass it as the mutation prop. */
<Connect mutation={mutationMap} render={(result) => /* Your UI here! */} />
```

Then, to use the mutation in your component, you'll need to let `Connect` know that it is an available field on the render prop object argument. To do this, use `[@bs.send]`.

```reason
open ReasonUrql;

[@bs.send] external likeDog : Connect.renderArgs(GetAllDogs.t, 'queryData, 'store) => {. "key": string } => unit = "";

let make = (_children) => {
  ...component,
  render: _ => <Connect
    mutation={mutationMap}
    renderProp={(result: Connect.renderArgs(GetAllDogs.t, 'queryData, 'store)) => {
      switch (result##response) {
      | Loading => <Loading />
      | Data(data) => {
          <div>
          {
            Array.map(
              dog =>
                switch (dog) {
                | Some(dog) =>
                  <Dog
                    key=dog##key
                    description=dog##description
                    id=dog##key
                    imageUrl=dog##imageUrl
                    name=dog##name
                    likes=dog##likes
                    onClick=result->likeDog
                  />
                | None => <div />
                },
              data##dogs,
            )
            |> ReasonReact.array
          }
          </div>
        }
      | Error(error) => <Error />
      }
    }}
  />
}
```

Ultimately, the use of `[@bs.send]` here is a workaround to support a proper binding of `urql`'s API and appropriate compilation by the BuckleScript compiler. It maintains type safety by allowing the user to specify the exact shape of each `mutation` they pass to `Connect`.

Typically, you'll set up your `queries` and `mutations` like the above examples and pass them as `props` to children of your `Connect`ed components.

#### `Connect` `props`

**`query`**

**type** – `option(Query.urqlQuery)`.

**description** – an instance of an `urql` query object.

**`mutation`**

**type** – `option(Js.Dict.t(Mutation.urqlMutation))`.

**description** – a `Js.Dict` mapping `urql` mutation objects to key names. Each `mutation` gets converted to a function under the hood by `urql` that is accessible on the object passed to `render`.

**`render`**

**type** - `renderArgs('data, 'queryData, 'store) => ReasonReact.reactElement`.

**description** – the render prop. Allows you to render UI with the results of `query`. All mutations supplied in `mutation` are available on the argument passed to `render`. `renderArgs` takes three type parameters:

`'data` – the structure of data expected to be returned by `query`. You can usually grab the `t` type of the GraphQL module used to create your `urql` query object.

`'queryData` – the structure of data stored in your cache. No need to define this if you don't plan on accessing any of your cache methods in your UI.

`'store` – the structure of your cache. No need to define this if you don't need to reference the cache in your UI.

**`cache`**

**type** – `bool`.

**description** – instruct `Connect` whether to cache query results. Default to `true`.

**`typeInvalidation`**

**type** - `bool`.

**description** - instruct `Connect` whether to use `typeNames` for cache invalidation. Defaults to `true`.

##### `Connect`'s `render` `prop`

`Connect`'s `render` `prop` is provided a `Js.t` object of type: `renderArgs('data, 'queryData, 'store)`. This object has the following fields:

**`response`**

**type** – `Loading | Data('data) | Error(error)`.

**description** – provides the state of your component and it's query as a variant. You can pattern match on these variant constructors to render different UI in each case.

**`fetching`**

**type** – `bool`.

**description** – indicates whether `urql` is fetching data from the GraphQL API.

**`loaded`** – `bool`.

**type** – `bool`.

**description** – indicates that `urql` has successfully loaded the response (either `Data('data)` or `Error(error)`).

**`data`**

**type** – `'data`.

**description** – the data returned by the GraphQL query, if any. `data` must match the the type parameter passed to `renderArgs`.

\*\*`error`

**type** – `{. "message": string })`.

**description** – the error returned by the GraphQL query, if any.

**`refetch`**

**type** – `(~options: refetchOptions, ~initial: bool=?) => unit`.

**description** – a function for refetching `query`. Use `~options=refetchOptions(~skipCache=true, ())` to skip the `cache` and hit the server directly.

**`refreshAllFromCache`**

**type** - `unit => unit`.

**description** – A function to refetch all queries from the cache.

**`cache`**
_Warning: experimental._

**type** – `{ invalidate: Query.urqlQuery => Client.invalidate, invalidateAll: unit => Client.invalidateAll, read: Query.urqlQuery => Client.read('queryData), update: ('store, string, 'queryData) => Client.update('queryData, 'store) }`.

**description** – functions for interacting with the cache.

## Getting Involved

This project is currently under active development. Please help out by [opening an issue](https://github.com/parkerziegler/reason-urql/issues) or [filing a PR](https://github.com/parkerziegler/reason-urql/pulls).
