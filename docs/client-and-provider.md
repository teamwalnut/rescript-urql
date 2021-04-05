# Client and Provider

The client is the central orchestrator of `rescript-urql`, and is responsible for executing queries, mutations, and subscriptions passed to `useQuery`, `useMutation`, and `useSubscription`. The `Provider` wraps the root of your application and passes your `rescript-urql` client instance, via React context, to hooks in your React tree.

## `Provider`

The `Provider`'s responsibility is to pass the `rescript-urql` client instance down to `useQuery`, `useMutation`, and `useSubcription` hooks through context. Wrap the root of your application with `Provider`.

You can access the `Provider` component by referencing `Context.Provider` after `open`ing `ReScriptUrql`.

### Props

| Prop    | Type       | Description                 |
| ------- | ---------- | --------------------------- |
| `value` | `Client.t` | The `urql` client instance. |

### Example

```rescript
open ReScriptUrql

let client = Client.make(~url="https://localhost:3000/graphql", ())

switch ReactDOM.querySelector("#root") {
| Some(el) => ReactDOM.render(<Context.Provider value=client><App /></Context.Provider>, el)
| None => ()
}
```

## Client

The client executes all requests in `rescript-urql` and delegates all incoming responses to subscribed hooks. Its full API is below; you can also look at its associated [interface file](../src/client/Client.rei).

### Client.make

Create an instance of an `urql` client.

```rescript
open ReScriptUrql

let client = Client.make(~url="https://localhost:3000/graphql", ())
```

`Client.make` accepts the following arguments:

| Argument          | Type                          | Description                                                                                                                                                                                                                                                                   |
| ----------------- | ----------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `url`             | `string`                      | The url of your GraphQL API.                                                                                                                                                                                                                                                  |
| `fetchOptions`    | `Client.fetchOptions('a)=?`   | Optional. A variant type representing fetch options to be used by your client. You can pass your `fetchOptions` as a plain `Fetch.requestInit` by wrapping it in the `Client.FetchOpts` tag, or instantiate it dynamically in a function wrapped by the `Client.FetchFn` tag. |
| `fetch`           | `fetchImpl('a)=?`             | Optional. A custom `fetch` implementation to use in lieu of `window.fetch`. For now, see [`__tests__/UrqlClient_test.re`](../__tests__/UrqlClient_test.re) for examples of how to use this option.                                                                            |
| `exchanges`       | `array(Exchanges.exchange)=?` | Optional. The array of exchanges to be used by your client.                                                                                                                                                                                                                   |
| `suspense`        | `bool=?`                      | Optional. A flag activating the experimental React suspense mode, which can be used during server-side rendering to prefetch data. Defaults to `false`.                                                                                                                       |
| `requestPolicy`   | `Types.requestPolicy=?`       | Optional. A polymorphic variant defining the default `requestPolicy` to use for all outgoing requests. Defaults to CacheFirst. Additional options include CacheOnly, NetworkOnly, and CacheAndNetwork. See [`Types.rei`](../src/Types.rei) for the full definition.           |
| `preferGetMethod` | `bool=?`                      | Optional. If `true`, will use the HTTP GET method rather than POST for operations of type `query`. Defaults to `false`.                                                                                                                                                       |
| `maskTypename`    | `bool=?`                      | Optional. If `true`, will apply the `maskTypename` utility to `data` returned on all operations. This makes the `__typename` properties non-enumerable.                                                                                                                       |

`Client.make` will return an instance of an `urql` client, represented by the abstract type `Client.t`.

#### Examples

**Create a client with just a `url`.**

```rescript
open ReScriptUrql;

let client = Client.make(~url="https://localhost:3000/graphql", ())
```

**Create a client with `fetchOptions`.**

```rescript
open ReScriptUrql

let fetchOptions = Fetch.RequestInit.make(
  ~method_=Post,
  ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
  (),
)

let client = Client.make(
  ~url="https://localhost:3000",
  ~fetchOptions=Client.FetchOpts(fetchOptions),
  (),
)
```

**Create a client with `exchanges`.**

```rescript
open ReScriptUrql

let client = Client.make(
  ~url="https://localhost:3000/graphql",
  ~exchanges=[
    Client.Exchanges.debugExchange,
    Client.Exchanges.dedupExchange,
    Client.Exchanges.cacheExchange,
    Client.Exchanges.fetchExchange
  ],
  ()
)
```

**Create a client with a non-default requestPolicy.**

```rescript
open ReScriptUrql

let client = Client.make(
  ~url="https://localhost:3000/graphql",
  ~requestPolicy=#CacheAndNetwork,
  ()
)
```

### `Client.executeQuery`

Imperatively execute a GraphQL query operation.

| Argument              | Type                          | Description                                                                                                                                             |
| --------------------- | ----------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `client`              | `Client.t`                    | The `urql` client instance.                                                                                                                             |
| `query`               | `(module Types.Operation)`    | The `graphql_ppx` module representing your GraphQL operation.                                                                                           |
| `additionalTypenames` | `array(string)=?`             | Optional. Flag that this operation depends on certain `__typename`s. Used by default in the document cache.                                             |
| `fetchOptions`        | `Fetch.requestInit=?`         | Optional. The fetch options to apply on the outgoing request.                                                                                           |
| `requestPolicy`       | `Types.requestPolicy=?`       | Optional. The request policy to use to execute the query. Defaults to `"cache-first"`.                                                                  |
| `url`                 | `string=?`                    | Optional. The GraphQL endpoint to use for the executing operation (if different from the one specified by `Client.make`).                               |
| `meta`                | `Types.operationDebugMeta=?`  | Optional. Add metadata that is only available in development with devtools.                                                                             |
| `suspense`            | `bool=?`                      | Optional. A flag activating the experimental React suspense mode, which can be used during server-side rendering to prefetch data. Defaults to `false`. |
| `preferGetMethod`     | `bool=?`                      | Optional. If `true`, will use the HTTP GET method rather than POST for operations of type `query`. Defaults to `false`.                                 |
| `variables`           | `Types.Operation.t_variables` | Optional. Variables to pass as part of your GraphQL query. This should be passed as the last positional argument, if needed.                            |

`client.executeQuery` returns a [`wonka` `source`](https://wonka.kitten.sh/api/sources) containing the results of executing the query. The result record has a variant type called `response` with constructors for `Data(d)`, `PartialData(d, e)`, `Error(e)`, and `Empty`, in addition to `data` and `error` fields for accessing the raw response values if desired.

| Return                                              | Description                                                                                                                             |
| --------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------- |
| `Wonka.Types.sourceT(Client.clientResponse('data))` | A `wonka` `source` containing a record of the results of query execution. Use the `response` field on this record for pattern matching. |

#### Example

```rescript
open ReScriptUrql

let client = Client.make(~url="https://localhost:3000/graphql", ())

module GetAllDogs = %graphql(`
  {
    dogs {
      name
      breed
      likes
    }
  }
`)

Client.executeQuery(~client, ~query=(module GetAllDogs), ())
  |> Wonka.subscribe((. data) => {
    switch({
      open Types
      data.response
    }) {
    | Data(d) => /* Access data returned from executing the request. */
    | Error(e) => /* Access any errors returned from executing the request. */
    | _ => /* Fallback if neither Data nor Error return information. */
    }
  })
```

### `Client.query`

The same as `Client.executeQuery`, but returns a `Js.Promise.t` rather than a `wonka` `source`. This method accepts all of the same arguments as `Client.executeQuery`.

| Return                                       | Description                                                                                                                           |
| -------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| `Js.Promise.t(Client.clientResponse('data))` | A `Js.Promise.t` containing a record of the results of query execution. Use the `response` field on this record for pattern matching. |

#### Example

```rescript
open ReScriptUrql

let client = Client.make(~url="https://localhost:3000/graphql", ());

module GetAllDogs = %graphql(`
  {
    dogs {
      name
      breed
      likes
    }
  }
`)

Client.query(~client, ~query=(module GetAllDogs), ())
  |> Js.Promise.then_(data => {
    switch({
      open Types
      data.response
    }) {
    | Data(d) => /* Access data returned from executing the request. */
    | Error(e) => /* Access any errors returned from executing the request. */
    | _ => /* Fallback if neither Data nor Error return information. */
    }
  });
```

### `Client.executeMutation`

Execute a GraphQL mutation operation.

| Argument              | Type                          | Description                                                                                                                                             |
| --------------------- | ----------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `client`              | `Client.t`                    | The `urql` client instance.                                                                                                                             |
| `mutation`            | `(module Types.Operation)`    | The `graphql_ppx` module representing your GraphQL operation.                                                                                           |
| `additionalTypenames` | `array(string)=?`             | Optional. Flag that this operation depends on certain `__typename`s. Used by default in the document cache.                                             |
| `fetchOptions`        | `Fetch.requestInit=?`         | Optional. The fetch options to apply on the outgoing request.                                                                                           |
| `requestPolicy`       | `Types.requestPolicy=?`       | Optional. The request policy to use to execute the query. Defaults to `"cache-first"`.                                                                  |
| `url`                 | `string=?`                    | Optional. The GraphQL endpoint to use for the executing operation (if different from the one specified by `Client.make`).                               |
| `meta`                | `Types.operationDebugMeta=?`  | Optional. Add metadata that is only available in development with devtools.                                                                             |
| `suspense`            | `bool=?`                      | Optional. A flag activating the experimental React suspense mode, which can be used during server-side rendering to prefetch data. Defaults to `false`. |
| `preferGetMethod`     | `bool=?`                      | Optional. If `true`, will use the HTTP GET method rather than POST for operations of type `query`. Defaults to `false`.                                 |
| `variables`           | `Types.Operation.t_variables` | Optional. Variables to pass as part of your GraphQL query. This should be passed as the last positional argument, if needed.                            |

`Client.executeMutation` returns a [`wonka` `source`](https://wonka.kitten.sh/api/sources) containing the results of executing the mutation. The result record has a variant type called `response` with constructors for `Data(d)`, `PartialData(d, e)`, `Error(e)`, and `Empty`, in addition to `data` and `error` fields for accessing the raw response values if desired.

| Return                                              | Description                                                                                                                                |
| --------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------ |
| `Wonka.Types.sourceT(Client.clientResponse('data))` | A `wonka` `source` containing a record of the results of mutation execution. Use the `response` field on this record for pattern matching. |

#### Example

```rescript
open ReScriptUrql

let client = Client.make(~url="https://localhost:3000/graphql", ())

module LikeDog = %graphql(`
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      name
      breed
      likes
    }
  }
`);

Client.executeMutation(
  ~client,
  ~mutation=(module LikeDog),
  {
    open LikeDog
    { key: "VmeRTX7j-" }
  }
) |> Wonka.subscribe((. data) => {
    switch {
      open Types
      data.response
    } {
    | Data(d) => /* Access data returned from executing the request. */
    | Error(e) => /* Access any errors returned from executing the request. */
    | _ => /* Fallback if neither Data nor Error return information. */
    }
  });
```

### `Client.mutation`

The same as `Client.executeMutation`, but returns a `Js.Promise.t` rather than a `wonka` `source`. This method accepts all of the same arguments as `Client.executeMutation`.

| Return                                       | Description                                                                                                                              |
| -------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------- |
| `Js.Promise.t(Client.clientResponse('data))` | A `Js.Promise.t` containing a record of the results of mutation execution. Use the `response` field on this record for pattern matching. |

#### Example

```rescript
open ReScriptUrql

let client = Client.make(~url="https://localhost:3000/graphql", ())

module LikeDog = %graphql(`
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      name
      breed
      likes
    }
  }
`);

Client.mutation(
  ~client,
  ~mutation=(module LikeDog),
  {
    open LikeDog
    { key: "VmeRTX7j-" }
  }
) |> Js.Promise.then_(data => {
    switch {
      open Types
      data.response
    } {
    | Data(d) => /* Access data returned from executing the request. */
    | Error(e) => /* Access any errors returned from executing the request. */
    | _ => /* Fallback if neither Data nor Error return information. */
    }
  });
```

### `Client.executeSubscription`

Execute a GraphQL subscription operation. If using the `executeSubscription` method, be sure your client is configured with the `subscriptionExchange`.

| Argument              | Type                          | Description                                                                                                                                             |
| --------------------- | ----------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `client`              | `Client.t`                    | The `urql` client instance.                                                                                                                             |
| `subscription`        | `(module Types.Operation)`    | The `graphql_ppx` module representing your GraphQL operation.                                                                                           |
| `additionalTypenames` | `array(string)=?`             | Optional. Flag that this operation depends on certain `__typename`s. Used by default in the document cache.                                             |
| `fetchOptions`        | `Fetch.requestInit=?`         | Optional. The fetch options to apply on the outgoing request.                                                                                           |
| `requestPolicy`       | `Types.requestPolicy=?`       | Optional. The request policy to use to execute the query. Defaults to `"cache-first"`.                                                                  |
| `url`                 | `string=?`                    | Optional. The GraphQL endpoint to use for the executing operation (if different from the one specified by `Client.make`).                               |
| `meta`                | `Types.operationDebugMeta=?`  | Optional. Add metadata that is only available in development with devtools.                                                                             |
| `suspense`            | `bool=?`                      | Optional. A flag activating the experimental React suspense mode, which can be used during server-side rendering to prefetch data. Defaults to `false`. |
| `preferGetMethod`     | `bool=?`                      | Optional. If `true`, will use the HTTP GET method rather than POST for operations of type `query`. Defaults to `false`.                                 |
| `variables`           | `Types.Operation.t_variables` | Optional. Variables to pass as part of your GraphQL query. This should be passed as the last positional argument, if needed.                            |

| Return                                              | Description                                                                                                                                    |
| --------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------- |
| `Wonka.Types.sourceT(Client.clientResponse('data))` | A `wonka` `source` containing a record of the results of subscription execution. Use the `response` field on this record for pattern matching. |

#### Example

```rescript
open ReScriptUrql

module SubscribeMessages = %graphql(`
  subscription subscribeMessages {
    newMessage {
      id
      message
    }
  }
`)

Client.executeSubscription(~client, ~subscription=(module SubscribeMessages), ())
  |> Wonka.subscribe((. data) => {
    switch({
      open Types
      data.response
    }) {
      | Data(d) => /* Access data returned from executing the request. */
      | Error(e) => /* Access any errors returned from executing the request. */
      | _ => /* Fallback if neither Data nor Error return information. */
    }
  });
```

### `Client.subscription`

An alias for `Client.executeSubscription`.

### `Client.readQuery`

Execute a GraphQL query, but immediately unsubscribe from the result. This method is particularly useful for reading directly from the cache without issuing a request to your GraphQL API. This method accepts all of the same arguments as `Client.executeQuery` and `Client.query`.

| Return                                 | Description                                                            |
| -------------------------------------- | ---------------------------------------------------------------------- |
| `option(Client.clientResponse('data))` | `Some(response)` if a result was found in the cache, otherwise `None`. |
