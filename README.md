# reason-urql

Reason bindings for Formidable's Universal React Query Library (`urql`) https://github.com/FormidableLabs/urql

> Warning: These bindings are in a WiP state. Do not use in production (yet) 😉.

## Table of Contents

- [What is `reason-urql`?](#what-is-reason-urql?)
- [Run the Example Project](#run-the-example-project)
- [Installation](#installation)
- [Building `reason-urql`](#building-reason-urql)
- [API](#api)
  - [`Client`](#client)
    - [Remaining Todos](#remaining-todos)
  - [`Provider`](#provider)
  - [`Connect`](#connect)
    - [Handling Errors](#handling-errors)
    - [Mutations and `Connect`](#mutations-and-connect)
  - [`Query`](#query)
  - [`Mutation`](#mutation)
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

let query =
  Query.query(
    ~query={|
query dogs {
  dogs {
    name
    breed
    description
  }
}
|},
    (),
  );

let client = Client.make(~url="http://localhost:3001", ());

Client.executeQuery(~client, ~query, ~skipCache=false)
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

#### Current API

| Name              | Type                                                                             | Description                               |
| ----------------- | -------------------------------------------------------------------------------- | ----------------------------------------- |
| `make`            | (~url: string, ~?fetchOptions: option(fetchOptions), unit)                       | Creates an `urql` Client.                 |
| `executeQuery`    | (~client: client, ~query: Query.urqlQuery, ~skipCache: bool) => Js.Promise.t('a) | Executes a `query` using the `client`.    |
| `executeMutation` | (~client: client, ~mutation: Mutation.urqlMutation) => Js.Promise.t('a)          | Executes a `mutation` using the `client`. |

#### Remaining Todos

- [ ] Add `graphql_ppx` for proper GraphQL schema validation. Tracked in [#5](https://github.com/parkerziegler/reason-urql/issues/5).
- [ ] Support properly typed custom `cache` implementations. Tracked in [#6](https://github.com/parkerziegler/reason-urql/issues/6).

### Provider

**Before reading this section, read the docs on `urql`'s [`Provider` API](https://github.com/FormidableLabs/urql#provider).**

To support the `Provider` component in ReasonReact, we take advantage of ReasonReact's excellent [`wrapJSForReason` helper](https://reasonml.github.io/reason-react/docs/en/interop#reasonreact-using-reactjs). `Provider` accepts a single `prop`, `client`. `client` accepts an instance of `Client.client` (see previous section). For example:

```reason
open ReasonUrql;

/* After instantiating our client (see above), we can wrap our app in the `Provider` component */
let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => <Provider client> <Header /> <Layout /> </Provider>,
};
```

### Connect

**Before reading this section, read the docs on `urql`'s [`Connect` API](https://github.com/FormidableLabs/urql#connect).**

Once you've wrapped your app in the `Provider` component, you can use `urql`'s `Connect` component to wire up UI to queries, mutations, and your cache. `Connect` uses the [render prop](https://reactjs.org/docs/render-props.html) pattern.

While `urql` names its render prop `children`, we opt to name it `renderProp` on the Reason side because `children` is a reserved keyword for ReasonReact components – naming a prop `children` will result in compiler errors. `Connect` provides an object to `renderProp`, which contains a set of known fields and a set of user-supplied mutations, provided in the `mutation` prop. The known fields are modeled as a `[bs.deriving abstract]`.

```reason
/* Types for the `renderProp` object. Notice that it accepts a type parameter `'data`, which requires the user to supply the structure of their data up front. */
[@bs.deriving abstract]
type renderArgs('data) = {
  fetching: bool,
  loaded: bool,
  data: 'data,
  error: Js.Nullable.t(error),
  refetch,
  refreshAllFromCache
};
```

To use the `Connect` component in Reason to execute a simple query and render UI with the results of that query, you can do the following:

```reason
open ReasonUrql;

/* Define the shape of data to pass as a type parameter to ReasonUrql.Connect.renderArgs. We require
you to type your data, providing type safety at compile time. */
[@bs.deriving abstract]
type dog = {
  key: string,
  name: string,
  breed: string,
  description: string
};

[@bs.deriving abstract]
type dogs = {
  dogs: array(dog)
};

/* An example make function for a ReasonReact component */
let make = (_children) => {
  ...component,
  render: _ => <Connect
    query={`Query(exampleQuery)}
    renderProp={(result: Connect.renderArgs(dogs)) => {

      /* We can use the fast pipe operator to access entries like `loaded`. */
      let loaded = result |. Connect.loaded;

      /* Now we can pattern match on the loaded variable and render UI. */
      switch (loaded) {
      | false => <div>(ReasonReact.string("Loading"))</div>
      | true => {
          <div>(
            Array.map((dog) => {
              <Dog
                key=(dog |. key)
                name=(dog |. name)
                breed=(dog |. breed)
                description=(dog |. description)
              />
            }, result |. Connect.data |. dogs)
            |> ReasonReact.array
          )</div>
        }
      }
    }}
  />
}
```

Awesome! We get the power of `Connect`'s render prop convention to connect our UI to a GraphQL query, all while maintaining type saftey with Reason 🚀.

#### Handling Errors

You may have noticed the `error` key on the object passed to `Connect`'s render prop. We bind to the error `urql`'s `Connect` API provides as a `Js.Nullable.t` and provide a simple converter function (`convertJsErrorToReason`) to transform this into a Reason `option` variant.

```reason
open ReasonUrql;

let error = Connect.convertJsErrorToReason(result |. Connect.error);
```

The result (`Some(error) | None`), if it evaluates to `Some(error)`, will provide you a `[@bs.deriving abstract]` with a `message` key. With this, you can render error specific UI in your components. For example, in your render prop function:

```reason
open ReasonUrql;

let error = Connect.convertJsErrorToReason(result |. Connect.error);

switch (error) {
  | Some(obj) =>
    <div>
      (
        ReasonReact.string(
          "Error: " ++ (obj |. Connect.message),
        )
      )
    </div>
  | None =>
    switch (loaded) => {
      | false => <div>(ReasonReact.string("Loading"))</div>
      | true => /* render your component */
    }
```

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
let likeDog = Mutation.mutation(
  ~query={|
    mutation likeDog($key: ID!) {
      likeDog(key: $key) {
        name
        key
        breed
        likes
      }
    }
  |},
  ()
);

let mutationMap = Js.Dict.empty();
Js.Dict.set(mutationMap, "likeDog", likeDog);

/* On your Connect component, pass it as the mutation prop. */
<Connect mutation={mutationMap} />
```

Then, to use the mutation in your component, you'll need to let `Connect` know that it is an available field on the render prop object argument. To do this, use `[@bs.send]`.

```reason
open ReasonUrql;

[@bs.send] external likeDog : Connect.renderArgs(dogs) => {. "key": string } => unit = "";

let make = (_children) => {
  ...component,
  render: _ => <Connect
    mutation={mutationMap}
    renderProp={(result: Connect.renderArgs(dogs)) => {
      let loaded = result |. Connect.loaded;
      let data = result |. Connect.data;
      switch (loaded) {
      | false => <div>(ReasonReact.string("Loading"))</div>
      | true => {
          <div style=(
            ReactDOMRe.Style.make(~display="grid", ~gridTemplateColumns="1fr 1fr 1fr", ())
          )>(
            Array.map((dog) => {
              <Dog
                key=(result |. key)
                /* Look here! likeDog is now accessible on result because of our [@bs.send]. We can
                pass it as a callback to our child component! */
                onClick=(result |. likeDog)
              />
            }, data |. dogs)
            |> ReasonReact.array
          )</div>
        }
      }
    }}
  />
}
```

Ultimately, the use of `[@bs.send]` here is a workaround to support a proper binding of `urql`'s API and appropriate compilation by the BuckleScript compiler. It maintains type safety by allowing the user to specify the exact shape of each `mutation` they pass to `Connect`.

### Query

**Before reading this section, read the docs on `urql`'s [`Query` API](https://github.com/FormidableLabs/urql#query).**

To use `reason-urql's` `query` API simply do something like the following:

```reason
open ReasonUrql;

let exampleQuery = Query.query(
  ~query={|
    query {
      dogs {
        key
        name
        breed
        description
      }
    }
  |},
  ()
);
```

Adding variables is as simple as adding the second named argument.

```reason
open ReasonUrql;

let exampleQuery = Query.query(
  ~query={|
    query getDog(key: ID!) {
      dog(key: $key) {
        name
        breed
        description
      }
    }
  |},
  ~variables={
    "key": "12345"
  },
  ()
);
```

We currently _don't_ typecheck the `variables` argument – the binding accepts a type parameter that allows `variables` to be of whatever type the user provided argument is. This may change in upcoming versions of `reason-urql`.

`Connect` allows you to pass either a single query or an array of queries as the `query` prop. To support this, we model the `query` prop in `Connect` as a polymorphic variant.

```reason
[
  | `Query(q)
  | `QueryArray(qa)
]
```

### Mutation

**Before reading this section, read the docs on `urql`'s [`Mutation` API](https://github.com/FormidableLabs/urql#mutation).**

The bindings for `reason-urql`'s `mutation` API is very similar to that of `query`. Just call the `mutation` function from the `Mutation` module.

```reason
open ReasonUrql;

let likeDog = Mutation.mutation(
  ~query={|
    mutation likeDog($key: ID!) {
      likeDog(key: $key) {
        name
        key
        breed
        likes
      }
    }
  |},
  ~variables={
    "key": "12345"
  },
  ()
);
```

Typically, you'll set up your `queries` and `mutations` like the above examples and pass them as `props` to your `Connect`ed components.

## Getting Involved

This project is currently under development. Please help out by [opening an issue](https://github.com/parkerziegler/reason-urql/issues) or [filing a PR](https://github.com/parkerziegler/reason-urql/pulls).
