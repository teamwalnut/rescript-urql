# reason-urql

Reason bindings for Formidable's Universal React Query Library (`urql`) https://github.com/FormidableLabs/urql

> Warning: These bindings are in a WiP state. Do not use in production (yet) 😉.

## Table of Contents

- [What is `reason-urql`?](#what-is-reason-urql?)
- [Run the Example Project](#run-the-example-project)
- [Installation](#installation)
- [Building `reason-urql`](#building-reason-urql)
- [API](#api)
  - [`Query`](#query)
    - [`Query` API](#query-api)
  - [`Mutation`](#mutation)
    - [`Mutation` API](#mutation-api)
  - [`Client`](#client)
    - [`Client` API](#client-api)
    - [Remaining Todos](#remaining-todos)
  - [`Provider`](#provider)
    - [`Provider` `props`](#provider-props)
  - [`Connect`](#connect)
    - [Pattern Matching with `response`](#handling-errors)
    - [Mutations and `Connect`](#mutations-and-connect)
- [Getting Involved](#getting-involved)

## What is `reason-urql`?

`urql` is a GraphQL client for React, allowing you to hook up your components to queries, mutations, and subscriptions. `reason-urql` provides bindings to `urql` that allow you to use the API in Reason. We are working hard to support the full API – check back regularly for updates.

## Run the Example Project

The example project is a simple app for viewing and liking Formidable dogs. To get the example running locally:

```sh
# in one terminal, start the GraphQL server
yarn start-demo-server

# in a second terminal, compile the app in watch mode
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

## Bulding `reason-urql`

To build a production version of `reason-urql`:

```sh
yarn run build
yarn run webpack:production
```

This will replace `build/Index.js` with an optimized build.

## API

### Query

**Before reading this section, read the docs on `urql`'s [`Query` API](https://github.com/FormidableLabs/urql#query).**

The `Query` `module` provides the core building block for genertaing queries in `reason-urql`. We use the power of [`graphql_ppx`](https://github.com/mhallin/graphql_ppx) to support well-typed GraphQL queries that will validate against your schema _at compile time_. 🎉

To generate a query, simply write your GraphQL query and call `reason-urql`s `query` function.

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

let query = Query.query(GetAllDogs.make());
```

Adding variables to your queries is as simple as passing them as named arguments to your `graphql_ppx` `module`'s `make` function.

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

let query = Query.query(GetDog.make(~key="VmeRTX7j-", ()));
```

In addition, there may be situations where you want to make a query but don't know the value of your variables ahead of time. To support this use case, we supply a [functor](https://reasonml.github.io/docs/en/module#module-functions-functors) in the `Query` module, simply called `Make`. It accepts a query module and returns a new module with your `urql` query attached. For example:

```reason
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

/* This will return the urql query object. */
let queryObject = GetDogQuery.query;

/* This will return the raw query string. */
let rawQuery = GetDogQuery.query -> Query.queryGet;
```

#### `Query` API

| Name                                        | Type                                                                      | Description                                                                                        |
| ------------------------------------------- | ------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- |
| `query`                                     | `{. "parse": Js.Json.t => 'a, "query": string, "variables": Js.Json.t} => |
| urqlQuery`| Creates an`urql``query` object. |
| `Make`                                      | `Module => Module` (functor)                                              | Accepts a graphql query module and returns a module with the `urql` `query` object attached to it. |

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

Just like the `Query` `module`, we also provide a functor called `Make` to support situations where you may not know the values of your variables when you need to construct your mutation.

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

/* This will return the urql mutation object. */
let mutationObject = LikeDogMutation.mutation;
```

This comes in very handy when wiring multiple mutations up to `Connect`. Check the example project for a sample use case.

#### `Mutation` API

| Name                                           | Type                                                                      | Description                                                                                              |
| ---------------------------------------------- | ------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------- |
| `mutation`                                     | `{. "parse": Js.Json.t => 'a, "query": string, "variables": Js.Json.t} => |
| urqlQuery`| Creates an`urql``mutation` object. |
| `Make`                                         | `Module => Module` (functor)                                              | Accepts a graphql mutation module and returns a module with the `urql` `mutation` object attached to it. |

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
     Js.log(dogs);
     Js.Promise.resolve(dogs);
   })
|> Js.Promise.catch(err => {
     Js.log2("Something went wrong!", err);
     Js.Promise.resolve(err);
   });
```

#### `Client` API

| Name              | Type                                                                               | Description                               |
| ----------------- | ---------------------------------------------------------------------------------- | ----------------------------------------- |
| `make`            | `(~url: string, ~?fetchOptions: option(fetchOptions), unit)`                       | Creates an `urql` Client.                 |
| `executeQuery`    | `(~client: client, ~query: Query.urqlQuery, ~skipCache: bool) => Js.Promise.t('a)` | Executes a `query` using the `client`.    |
| `executeMutation` | `(~client: client, ~mutation: Mutation.urqlMutation) => Js.Promise.t('a)`          | Executes a `mutation` using the `client`. |

#### Remaining Todos

- [ ] Support properly typed custom `cache` implementations. Tracked in [#6](https://github.com/parkerziegler/reason-urql/issues/6).

### Provider

**Before reading this section, read the docs on `urql`'s [`Provider` API](https://github.com/FormidableLabs/urql#provider).**

To support the `Provider` component in ReasonReact, we take advantage of ReasonReact's excellent [`wrapJSForReason` helper](https://reasonml.github.io/reason-react/docs/en/interop#reasonreact-using-reactjs). `Provider` accepts a single `prop`, `client`. `client` accepts an instance of `Client.client` (see previous section). For example:

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

| Name     | Type            | Description                                                     |
| -------- | --------------- | --------------------------------------------------------------- |
| `client` | `Client.client` | The `client` instance that the `Provider` component should use. |

### Connect

**Before reading this section, read the docs on `urql`'s [`Connect` API](https://github.com/FormidableLabs/urql#connect).**

Once you've wrapped your app in the `Provider` component, you can use `urql`'s `Connect` component to wire up UI to queries, mutations, and your cache. `Connect` uses the [render prop](https://reactjs.org/docs/render-props.html) pattern.

While `urql` names its render prop `children`, we opt to name it `render` on the Reason side because `children` is a reserved keyword for ReasonReact components – naming a prop `children` will result in compiler errors. `Connect` provides a `Js.t` object to `render`, which contains a set of known fields and a set of user-supplied mutations, provided in the `mutation` prop. We use a simple `Js.t` object and `Js.Obj.assign` to grab the user-supplied mutations from `urql`.

```reason
/* Types for the object supplied to `render`. User-supplied mutations will be spread into this object and available as functions. */
type renderArgs('data) = {
  .
  "response": response('data),
  "fetching": bool,
  "loaded": bool,
  "data": option('data),
  "error": option(error),
  "refetch": refetch,
  "refreshAllFromCache": refreshAllFromCache,
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
    query={`Query(query)}
    render={(result: Connect.renderArgs(GetAllDogs.t)) =>
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

You may have noticed in the above example that `result##response` returns a variant containing three constructors, `Loading`, `Data(data)`, and `Error(error)`. This makes rendering conditional UI a breeze in ReasonReact. Each constructor will be passed the proper payload, whether that is `data` matching the user-supplied `'data` type or `error` of the form `{. "message": string })`.

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

[@bs.send] external likeDog : Connect.renderArgs(dogs) => {. "key": string } => unit = "";

let make = (_children) => {
  ...component,
  render: _ => <Connect
    mutation={mutationMap}
    renderProp={(result: Connect.renderArgs(GetAllDogs.t)) => {
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

Typically, you'll set up your `queries` and `mutations` like the above examples and pass them as `props` to your `Connect`ed components.

#### `Connect` `props`

| Name               | Type                                                                           | Description                                                                                 |
| ------------------ | ------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------- |
| `query`            | `option([Query(UrqlQuery.urqlQuery) | QueryArray(array(UrqlQuery.urqlQuery))]` | An `urql` `query` object or array of `query` objects.                                       |
| `mutation`         | `option(mutationMap)`                                                          | A map of user-supplied `mutations`. These will be spread on the object passed to `render`.  |
| `render`           | `renderArgs('data) => ReasonReact.reactElement`                                | A render prop for rendering UI with the result of executing supplied queries and mutations. |
| `cache`            | `bool`                                                                         | Instruct `Connect` to cache queries. Defaults to `true`.                                    |
| `typeInvalidation` | `bool`                                                                         | Instruct `Connect` to invalidate the cache using `typeNames`. Defaults to `true`.           |

## Getting Involved

This project is currently under active development. Please help out by [opening an issue](https://github.com/parkerziegler/reason-urql/issues) or [filing a PR](https://github.com/parkerziegler/reason-urql/pulls).
