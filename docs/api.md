# API

This document provides all of the API documentation for `reason-urql`. For more on how to get started, check out our [Getting Started](/docs/getting-started.md) guide.

## Components

### `Query`

The `Query` component is the core building block of `reason-urql`. Use this component to query your GraphQL API and render UI with the result.

#### Props

| Prop            | Type                          | Description                                                                  |
| --------------- | ----------------------------- | ---------------------------------------------------------------------------- |
| `query`         | `string`                      | The GraphQL query to execute.                                                |
| `variables`     | `option(Js.Json.t)`           | Variables to be passed to the GraphQL query.                                 |
| `requestPolicy` | `option(Types.requestPolicy)` | The request policy to use to execute the query. Defaults to `"cache-first"`. |

#### Render Props

| Prop           | Type                                                       | Description                                                                                                                                    |
| -------------- | ---------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------- |
| `fetching`     | `bool`                                                     | A boolean flag to indicate if the request is currently executing.                                                                              |
| `data`         | `'a`                                                       | The data returned by your GraphQL API.                                                                                                         |
| `error`        | `CombinedError.t`                                          | The error(s), if any, returned by the GraphQL operation.                                                                                       |
| `executeQuery` | `option(Js.Json.t) => Js.Promise.t(Types.operationResult)` | A callback to manually execute the query operation. Useful to imperatively execute the query. Accepts variables required to execute the query. |
| `response`     | `Types.response('a)`                                       | A variant containing constructors for `Data`, `Error`, `Fetching` and `NotFound`. Useful for pattern matching to render conditional UI.        |

#### Example

```reason
open ReasonUrql;

module GetAllDogs = [%graphql
  {|
  query dogs {
    dogs {
      name
      breed
      likes
    }
  }
|}
];

let query = GetAllDogs.make()##query;

let component = ReasonReact.statelessComponent("Example");

let make = (_children) => {
  ...component,
  render: _self => {
    <Query query>
      {...({ response }): Query.queryRenderProps(GetAllDogs.t) => {
        switch (response) {
          | Fetching => /* Render loading UI. */
          | Data(d) => /* Render UI with data. */
          | Error(e) => /* Render error handling UI. */
          | NotFound => /* Render fallback UI. This may occur if your GraphQL server is not running. */
        }
      }}
    </Query>
  }
}
```

**Check out the `2-query` example in the `examples` directory to learn more about using the `Query` component.**

### `Mutation`

The `Mutation` component allows you to imperatively execute mutations. Use this component to pass the `executeMutation` function to your UI.

#### Props

| Prop    | Type     | Description                      |
| ------- | -------- | -------------------------------- |
| `query` | `string` | The GraphQL mutation to execute. |

#### Render Props

| Prop              | Type                                                       | Description                                                                                                                                             |
| ----------------- | ---------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `fetching`        | `bool`                                                     | A boolean flag to indicate if the request is currently executing.                                                                                       |
| `data`            | `'a`                                                       | The data returned by your GraphQL API.                                                                                                                  |
| `error`           | `CombinedError.t`                                          | The error(s), if any, returned by the GraphQL operation.                                                                                                |
| `executeMutation` | `option(Js.Json.t) => Js.Promise.t(Types.operationResult)` | A callback to manually execute the mutation operation. Useful to imperatively execute the mutation. Accepts variables required to execute the mutation. |
| `response`        | `Types.response('a)`                                       | A variant containing constructors for `Data`, `Error`, `Fetching` and `NotFound`. Useful for pattern matching to render conditional UI.                 |

#### Example

```reason
open ReasonUrql;

module LikeDog = [%graphql
  {|
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      likes
    }
  }
|}
];

let request = GetAllDogs.make(~key="VmeRTX7j-", ());
let mutation = request##query;
let variables = request##variables;

let component = ReasonReact.statelessComponent("Example");

let make = (_children) => {
  ...component,
  render: _self => {
    <Mutation query=mutation>
      {...({ executeMutation }): Mutation.mutationRenderProps(LikeDog.t) => {
        <button onClick={(_e) => executeMutation(Some(variables))}>
          "Click Me to Execute the Mutation" -> ReasonReact.string
        </button>
      }}
    </Mutation>
  }
}
```

**Check out the `3-mutation` example in the `examples` directory to learn more about using the `Mutation` component.**

### `Subscription`

The `Subscription` component allows you to subscribe to GraphQL subscriptions emitted by your API. Use this component to render UI with realtime data.

#### Props

| Prop        | Type                                                        | Description                                                                                                                                |
| ----------- | ----------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------ |
| `query`     | `string`                                                    | The GraphQL subscription to execute.                                                                                                       |
| `variables` | `option(Js.Json.t)`                                         | Variables to be passed to the GraphQL subscription.                                                                                        |
| `handler`   | `(~prevSubscriptions: option('a), ~subscription: 'b) => 'a` | A function to control how to aggregate subscription results, given the accumulation of `prevSubscriptions` and the current `subscription`. |

#### Render Props

| Prop       | Type                 | Description                                                                                                                             |
| ---------- | -------------------- | --------------------------------------------------------------------------------------------------------------------------------------- |
| `fetching` | `bool`               | A boolean flag to indicate if the request is currently executing.                                                                       |
| `data`     | `'a`                 | The data returned by your GraphQL API.                                                                                                  |
| `error`    | `CombinedError.t`    | The error(s), if any, returned by the GraphQL operation.                                                                                |
| `response` | `Types.response('a)` | A variant containing constructors for `Data`, `Error`, `Fetching` and `NotFound`. Useful for pattern matching to render conditional UI. |

### `Provider`

The `Provider`'s responsibility is to pass the `urql` client instance down to `Query`, `Mutation`, and `Subscription` components through context. Wrap the root of your application with `Provider`.

#### Props

| Prop     | Type       | Description                 |
| -------- | ---------- | --------------------------- |
| `client` | `Client.t` | The `urql` client instance. |

#### Example

```reason
open ReasonUrql;

let client = Client.make(~url="https://localhost:3000/graphql", ());

ReactDOMRe.renderToElementWithId(<Provider client><App /></Provider>, "root");
```

## `Client`

The client is the central orchestrator of `reason-urql`, and is responsible for executing queries, mutations, and subscriptions passed to the `Query`, `Mutation`, and `Subscription` components.

To create a client, simple call the `make` function from the `Client` module.

```reason
open ReasonUrql;

let client = Client.make(~url="https://localhost:3000/graphql", ());
```

The client optionally accepts a `fetchOptions` argument for passing additional properties to `fetch` requests (i.e. authorization headers) and an `exchanges` argument for customizing the exchanges that operations are passed through.

### `Client.make`

Instantiate an `urql` client instance.

```reason
(
  ~url: string,
  ~fetchOptions: option(fetchOptions)=?,
  ~exchanges: option(array(Exchanges.exchange))=?,
  unit
) => t
```

| Argument       | Type                                  | Description                                       |
| -------------- | ------------------------------------- | ------------------------------------------------- |
| `url`          | `string`                              | The url of your GraphQL API.                      |
| `fetchOptions` | `option(fetchOptions)=?`              | Optional fetch options to be used by your client. |
| `exchanges`    | `option(array(Exchanges.exchange))=?` | The array of exchanges to be used by your client. |

**Create a client with just a `url`.**

```reason
open ReasonUrql;

let client = Client.make(~url="https://localhost:3000/graphql", ());
```

**Create a client with `fetchOptions`.**

```reason
open ReasonUrql;

let fetchOptions =
  Client.FetchOpts(Fetch.RequestInit.make(
    ~method_=Post,
    ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
    (),
  ));

let client = Client.make(~url="https://localhost:3000/graphql", ~fetchOptions, ());
```

**Create a client with `exchanges`.**

```reason
open ReasonUrql;

let client = Client.make(
  ~url="https://localhost:3000/graphql",
  ~exchanges=[|
    Exchanges.fetchExchange,
    Exchanges.dedupExchange,
    Exchanges.cacheExchange,
    Exchanges.debugExchange
  |], ());
```

#### `Client.executeQuery` - execute a GraphQL query operation.

```reason
(
  ~client: t,
  ~query: Types.graphqlRequest,
  ~opts: option(Types.partialOperationContext)=?,
  unit
) =>
Wonka.Types.sourceT('a) =
"";
```

| Argument | Type                                      | Description                                                                                                  |
| -------- | ----------------------------------------- | ------------------------------------------------------------------------------------------------------------ |
| `client` | `Client.t`                                | The `urql` client instance.                                                                                  |
| `query`  | `Types.graphqlRequest`                    | The GraphQL query request object, consisting of a `query` string and, optionally, a `variables` `Js.Json.t`. |
| `opts`   | `option(Types.partialOperationContext)=?` | Additional options to pass to the operation.                                                                 |

**Execute a GraphQL query.**

```reason
open ReasonUrql;

let client = Client.make(~url="https://localhost:3000/graphql", ());

module GetAllDogs = [%graphql
  {|
  query dogs {
    dogs {
      name
      breed
      likes
    }
  }
|}
];

let query = Request.createRequest(~query=GetAllDogs.make()##query, ());

Client.executeQuery(~client, ~query, ())
  |> Wonka.subscribe((. result) => {
    Js.log2("Result: ", result);
  });
```

#### `Client.executeMutation` - execute a GraphQL mutation operation.

```reason
(
  ~client: t,
  ~mutation: UrqlTypes.graphqlRequest,
  ~opts: UrqlTypes.partialOperationContext=?,
  unit
) =>
Wonka.Types.sourceT('a) =
"";
```

| Argument   | Type                                      | Description                                                                                                     |
| ---------- | ----------------------------------------- | --------------------------------------------------------------------------------------------------------------- |
| `client`   | `Client.t`                                | The `urql` client instance.                                                                                     |
| `mutation` | `Types.graphqlRequest`                    | The GraphQL mutation request object, consisting of a `query` string and, optionally, a `variables` `Js.Json.t`. |
| `opts`     | `option(Types.partialOperationContext)=?` | Additional options to pass to the operation.                                                                    |

**Execute a GraphQL mutation.**

```reason
open ReasonUrql;

let client = Client.make(~url="https://localhost:3000/graphql", ());

module LikeDog = [%graphql
  {|
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      name
      breed
      likes
    }
  }
|}
];

let mutation = LikeDog.make(~key="VmeRTX7j-", ());

let mutationRequest =
  Request.createRequest(
    ~query=mutation##query,
    ~variables=mutation##variables,
    (),
  );

Client.executeQuery(~client, ~mutation=mutationRequest, ())
  |> Wonka.subscribe((. result) => {
    Js.log2("Result: ", result);
  });
```

#### `Client.executeSubscription` - execute a GraphQL subscription operation.

```reason
(
  ~client: t,
  ~subscription: UrqlTypes.graphqlRequest,
  ~opts: option(UrqlTypes.partialOperationContext)=?,
  unit
) =>
Wonka.Types.sourceT('a) =
"";
```

| Argument       | Type                                      | Description                                                                                                         |
| -------------- | ----------------------------------------- | ------------------------------------------------------------------------------------------------------------------- |
| `client`       | `Client.t`                                | The `urql` client instance.                                                                                         |
| `subscription` | `Types.graphqlRequest`                    | The GraphQL subscription request object, consisting of a `query` string and, optionally, a `variables` `Js.Json.t`. |
| `opts`         | `option(Types.partialOperationContext)=?` | Additional options to pass to the operation.                                                                        |

**Execute a GraphQL subscription.**

```reason
open ReasonUrql;

let client = Client.make(~url="https://localhost:3000/graphql", ());

module SubscribeMessages = [%graphql
  {|
  subscription subscribeMessages {
    newMessage {
      id
      message
    }
  }
|}
];

let subscription = Request.createRequest(~query=SubscribeMessages.make()##query, ());

Client.executeSubscription(~client, ~subscription, ())
  |> Wonka.subscribe((. result) => {
    Js.log2("Result: ", result);
  });
```
