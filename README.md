# reason-urql

ReasonML bindings for Formidable's Universal React Query Library (`urql`) https://github.com/FormidableLabs/urql

> Warning: These bindings are in a WiP state. Do not use in production (yet) 😉.

## Table of Contents

- [What is `reason-urql`?](#what-is-reason-urql?)
- [Run the Example Project](#run-the-example-project)
- [Installation](#installation)
- [Building `reason-urql`](#building-reason-urql)
- [API](#api)
  - [`Client`](#client)
  - [`Provider`](#provider)
  - [`Connect`](#connect)

## What is `reason-urql`?

`urql` is a GraphQL client for React, allowing you to hook up your components to queries, mutations, and subscriptions. `reason-urql` provides bindings to `urql` that allow you to use the API in Reason. We are working hard to support the full API – check back regularly for updates.

## Run the Example Project

The example project is a simple app for viewing and liking Formidable dogs. To get the example running locally:

```sh
# in one terminal, start the GraphQL server
yarn run start-demo-server

# in another terminal, start webpack for the app
yarn run start-demo-app

# then, in a third terminal
open example/app/index.html
```

You should now be able to edit the example freely, and refresh the page to see changes take effect. We're working on getting hot reloading into the example soon!

## Installation

These bindings have not yet been published to `npm`. Check back soon for updates!

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

`urql`'s `Client` API takes a config object containing values for `url`, `cache`, `initialCache`, and `fetchOptions`. We model this config as a `[@bs.deriving abstract]`, BuckleScript's [implementation for JavaScript objects](https://bucklescript.github.io/docs/en/object#record-mode). Note that using `[@bs.deriving abstract]` gives us both a `type` for `urqlClientConfig` and a function for generating the config object (also called `urqlClientConfig`). To create a new `Client` in `reason-urql`, first create the config:

```re
let config: ReasonUrql.Client.urqlClientConfig(Js.t({.})) = ReasonUrql.Client.urqlClientConfig(~url="https://myapi.com", ());
```

The type argument passed to `Client.urqlClientConfig` allows you to specify the structure of your `fetchOptions` argument. This argument can be either an object or a function that returns an object. In Reason, we model this using a polymorphic variant:

```re
fetchOptions: [
  | `FetchObj(Js.t('fetchOptions))
  | `FetchFunc(unit => Js.t('fetchOptions))
]
```

To pass something like a custom header to your `fetch` calls, for example, you'd do the following:

```re
/* Define the type matching the structure of your `fetchOptions`. */
type fetchOptions = {.
  "custom-header": string
}

/* Use the polymorphic variant `FetchObj to mark `fetchOptions` as a plain object. */
let fetchOptions = `FetchObj({. "custom-header": "my-custom-header" });

/* We can take advantage of Reason punning to pass `fetchOptions` implictly. */
let config: ReasonUrql.Client.urqlClientConfig(fetchOptions) = ReasonUrql.Client.urqlClientConfig(~url="https://myapi.com", ~fetchOptions, ());
```

This allows you to get type safety for your `fetchOptions` argument. The compiler will also alert you if you alter the shape of `fetchOptions` without updating the type appropriately.

Now that we have a config, creating the `Client` is easy:

```re
let client = ReasonUrql.Client.client(config);
```

Once the `Client` is instantiated, we get access to its methods `executeQuery` and `executeMutation`. Since these APIs are `Promise`-based on the JS side of things, you'll need to use [Reason's `Promise` syntax](https://reasonml.github.io/docs/en/promise) to use them. For example:

```
let exampleQuery = ReasonUrql.Query.urqlQuery(
  ~query={|
    query {
      dogs {
        name
        breed
        description
      }
    }
  |},
  ()
);

Client.executeQuery(client, exampleQuery, false)
|>  Js.Promise.then_(value => {
    let dogs = value##data##dogs;
    Js.log(dogs);
    Js.Promise.resolve(dogs);
});
```

#### Remaining Todos

PRs welcome!

- [] Support properly typed custom `cache` implementations. Tracked in [#6](https://github.com/parkerziegler/reason-urql/issues/6).
- [] Potentially require type parameters to properly type the results of `executeQuery` and `executeMutation` rather than leaving them abstract.

### Provider

**Before reading this section, read the docs on `urql`'s [`Provider` API](https://github.com/FormidableLabs/urql#provider).**

To support the `Provider` component in ReasonReact, we take advantage of ReasonReact's excellent [`wrapJSForReason` helper](https://reasonml.github.io/reason-react/docs/en/interop#reasonreact-using-reactjs). `Provider` accepts a single `prop`, `client`. `client` accepts an instance of `ReasonUrql.Client.client` (see previous section). For example:

```
/* After instantiating our client (see above), we can wrap our app in the `Provider` component */
let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => <ReasonUrql.Provider client> <Header /> <Layout /> </ReasonUrql.Provider>,
};
```

### Connect

**Before reading this section, read the docs on `urql`'s [`Connect` API](https://github.com/FormidableLabs/urql#connect).**

Once you've wrapped your app in the `Provider` component, you can use `urql`'s `Connect` component to wire up UI to queries, mutations, and your cache. `Connect` uses the [render prop](https://reactjs.org/docs/render-props.html) pattern.

While `urql` names its render prop `children`, we opt to name it `renderProp` on the Reason side because `children` is a reserved keyword for ReasonReact components – naming a prop `children` will result in compiler errors. `Connect` provides an object to `renderProp`, which contains a set of known fields and a set of user-supplied mutations, provided in the `mutation` prop. The known fields are modeled as a `[bs.deriving abstract]`.

```re
/* Types for the `renderProp` object. Notice that it accepts a type parameter 'data, which requires the user to supply the structure of their data up front. */
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

```re
/* Define a GraphQL query with urql's query API (see below) */
let exampleQuery = ReasonUrql.Query.urqlQuery(
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

/* Define the shape of data to pass as a type parameter to Connect.renderArgs. We require
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
  render: _ => <ReasonUrql.Connect
    query={`Query(exampleQuery)}
    renderProp={(result: ReasonUrql.Connect.renderArgs(dogs)) => {

      /* We can use the fast pipe operator to access entries like `loaded`.
      This is due to the fact that `renderArgs` is a [@bs.deriving abstract]. */
      let loaded = result |. ReasonUrql.Connect.loaded;

      /* Now we can pattern match on the loaded variable and render UI. */
      switch (loaded) {
      | false => <div>(ReasonReact.string("Loading"))</div>
      | true => {
          <div>(
            Array.map((dog) => {
              let key = dog |. key;
              <Dog
                key={j|$key|j}
                name={dog |. name}
                breed={dog |. breed}
                description={dog |. description}
              />
            }, result |. ReasonUrql.Connect.data |. dogs)
            |> ReasonReact.array
          )</div>
        }
      }
    }}
  />
}
```

Awesome! We get the power of `Connect`'s render prop convention to connect our UI to a GraphQL query, all while maintaining type saftey with Reason 🚀.

#### Mutation

`urql` does a pretty nifty thing to support mutations on the `Connect` component. It takes a user-supplied `mutation` map and turns each mutation into a function accessible on the object passed to `Connect`'s render prop. It accomplishes this through props spreading, [a technique that Reason does not support](https://reasonml.github.io/reason-react/docs/en/props-spread). Props spreading can be modeled easily on the TypeScript side using intersection types. It gets a bit trickier to model this behavior on the Reason end while still maintaining robust type safety. We can't intersect a set of known types (the render prop argument) with unknown, user-supplied types (mutations) behind the scenes – only the user can know what they need and how it's typed.

To alleviate some of this difficulty, we use BuckleScript's [`JS.Dict` API](https://bucklescript.github.io/docs/en/object#hash-map-mode) to model the `mutation` prop.

```re
/* ReasonUrql.Mutation.urqlMutation is just `urql`'s Mutation API (see below) */
type mutationMap = Js.Dict.t(ReasonUrql.Mutation.urqlMutation);
```

To set up a mutation map, you'd do something like the following:

```re
/* Define a GraphQL mutation. */
let likeDog: ReasonUrql.Mutation.urqlMutation = ReasonUrql.Mutation.mutation(
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
<ReasonUrql.Connect mutation={mutationMap} />
```

Then, to use the mutation in your component, you'll need to let `Connect` know that it is an available field on the render prop object argument. To do this, use `[@bs.send]`.

```re
/* Here we are saying that there is a function (mutation) on the `renderArgs` object called `likeDog`. This function accepts a JS object containing a `key` of type string. */
[@bs.send] external likeDog : Connect.renderArgs(dogs) => {. "key": string } => unit = "";
```

Then, to use it in your component:

```re
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
              let key = dog |. key;
              <Dog
                key={j|$key|j}
                name={dog |. name}
                breed={dog |. breed}
                description={dog |. description}
                /* Look here! likeDog is now accessible on result because of our [@bs.send]. We can
                pass it as a callback to our child component! */
                onClick={result |. likeDog}
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

**Before reading this section, read the docs on `urql`'s native JS [`Query` API](https://github.com/FormidableLabs/urql#query).**

Binding to `urql`'s `query` API is fairly simple. The JS implementation is just a function that accepts a `query` string argument (your GraphQL query) and, optionally, a `variables` argument and returns an object of the shape:

```js
{
  query: string,
  variables: object
}
```

To use `reason-urql's` `query` API simply do something like the following:

```re
let exampleQuery = Query.urqlQuery(
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

```re
let exampleQuery: Query.urqlQuery = Query.query(
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
    "key": "uuid-of-dog"
  },
  ()
);
```

We currently _don't_ typecheck the `variables` argument – the binding accepts a type parameter that allows `variables` to be of whatever type the user provided argument is. This may change in upcoming versions of `reason-urql`.

### Mutation

**Before reading this section, read the docs on `urql`'s native JS [`Mutation` API](https://github.com/FormidableLabs/urql#mutation).**

The bindings for `reason-urql`'s `mutation` API is very similar to that of `query`. Just call the `mutation` function from the `Mutation` module.

```re
let likeDog: Mutation.urqlMutation = Mutation.mutation(
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
    "key": "uuid-of-dog"
  },
  ()
);
```

Typically, you'll set up your `queries` and `mutations` like the above examples and pass them as `props` to your `Connect`ed components.
