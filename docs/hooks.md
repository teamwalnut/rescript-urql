# Hooks

In this section, we cover the main mechanism for requesting data in `reason-urql` – hooks!

`reason-urql` comes with a set of custom hooks to use in your ReasonReact components, including `useQuery`, `useMutation`, `useDynamicMutation`, and `useSubscription`. These are fully type safe and will automatically infer the type of your GraphQL response if using `graphql_ppx_re` or `graphql_ppx`.

## `useQuery`

`useQuery` allows you to execute a GraphQL query.

### Arguments

| Argument          | Type                          | Description                                                                                                                                             |
| ----------------- | ----------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `query`           | `(module Types.Operation)`    | The `graphql_ppx` module representing your GraphQL operation.                                                                                           |
| `requestPolicy`   | `Types.requestPolicy=?`       | Optional. The request policy to use to execute the query. Defaults to `"cache-first"`.                                                                  |
| `pause`           | `bool=?`                      | Optional. A boolean flag instructing `useQuery` to pause execution of the query operation.                                                              |
| `fetchOptions`    | `Fetch.requestInit=?`         | Optional. The fetch options to apply on the outgoing request.                                                                                           |
| `requestPolicy`   | `Types.requestPolicy=?`       | Optional. The request policy to use to execute the query. Defaults to `"cache-first"`.                                                                  |
| `url`             | `string=?`                    | Optional. The GraphQL endpoint to use for the executing operation (if different from the one specified by `Client.make`).                               |
| `pollInterval`    | `int=?`                       | Optional. Instructs the client to reexecute the query every `pollInterval` milliseconds.                                                                |
| `meta`            | `Types.operationDebugMeta=?`  | Optional. Add metadata that is only available in development with devtools.                                                                             |
| `suspense`        | `bool=?`                      | Optional. A flag activating the experimental React suspense mode, which can be used during server-side rendering to prefetch data. Defaults to `false`. |
| `preferGetMethod` | `bool=?`                      | Optional. If `true`, will use the HTTP GET method rather than POST for operations of type `query`. Defaults to `false`.                                 |
| `variables`       | `Types.Operation.t_variables` | Optional. Variables to pass as part of your GraphQL query. This should be passed as the last positional argument, if needed.                            |

### Return Type

`useQuery` returns a tuple containing the result of executing your GraphQL query and a function for re-executing the query imperatively.

| Return Value   | Type                                                                                                                                                                                                              | Description                                                                                                                                                                                                                                                           |
| -------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `state`        | `Hooks.hookResponse('response)`                                                                                                                                                                                   | A record containing fields for `fetching`, `data`, `error`, `response`, `extensions`, and `stale`. `response` is a variant containing constructors for `Data`, `PartialData`, `Error`, `Fetching`, and `Empty`. Useful for pattern matching to render conditional UI. |
| `executeQuery` | `(~fetchOptions: Fetch.requestInit=?, ~requestPolicy: Types.requestPolicy=?, ~url: string=?, ~pollInterval: int=?, ~meta: Types.operationDebugMeta=?, ~suspense: bool=?, ~preferGetMethod: bool=?, unit) => unit` | A function for imperatively re-executing the query. Accepts labelled arguments for modifying specific conditions of the query execution.                                                                                                                              |

### Example

```reason
open ReasonUrql;

module GetPokémon = [%graphql
  {|
  query pokémon($name: String!) {
    pokemon(name: $name) {
      name
      classification
      image
    }
  }
|}
];

[@react.component]
let make = () => {
  let (Hooks.{response}, reexecuteQuery) =
    Hooks.useQuery(~query=(module GetPokemon), {name: "Butterfree"});

  switch (response) {
    | Fetching => <div> "Loading"->React.string </div>
    | Data(d)
    | PartialData(d, _e) =>
      <div>
         <img src=d.pokemon.image>
         <span> d.pokemon.name->React.string </span>
         <span> d.pokemon.classification->React.string </span>
         <button
          onClick={_e =>
            reexecuteQuery(
              ~requestPolicy=`NetworkOnly,
              ()
            )
          }
         >
          {j|Refetch data for $d.pokemon.name|j} -> React.string
         </button>
      </div>
    | Error(_e) => <div> "Error"->React.string </div>
    | Empty => <div> "Empty"->React.string </div>
  }
}
```

Check out `examples/2-query` to see an example of using the `useQuery` hook.

## `useMutation`

`useMutation` allows you to execute a GraphQL mutation via the returned `executeMutation` function.

### Arguments

| Argument   | Type                       | Description                                                   |
| ---------- | -------------------------- | ------------------------------------------------------------- |
| `mutation` | `(module Types.Operation)` | The `graphql_ppx` module representing your GraphQL operation. |

### Return Type

`useMutation` returns a tuple containing the result of executing your GraphQL mutation and a function for executing the mutation imperatively, `executeMutation`. By default, `useMutation` **does not** execute your mutation when your component renders – rather, it is up to you to call `executeMutation` when you want to by attaching it to on an event handler or running it inside of an effect. Read more on the [`urql` docs](https://formidable.com/open-source/urql/docs/basics/mutations/#sending-a-mutation).

| Return Value      | Type                                                                                                                                                                                                                                                          | Description                                                                                                                                                                                                                                                           |
| ----------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `state`           | `Hooks.hookResponse('response)`                                                                                                                                                                                                                               | A record containing fields for `fetching`, `data`, `error`, `response`, `extensions`, and `stale`. `response` is a variant containing constructors for `Data`, `PartialData`, `Error`, `Fetching`, and `Empty`. Useful for pattern matching to render conditional UI. |
| `executeMutation` | `(~fetchOptions: Fetch.requestInit=?, ~requestPolicy: Types.requestPolicy=?, ~url: string=?, ~pollInterval: int=?, ~meta: Types.operationDebugMeta=?, ~suspense: bool=?, ~preferGetMethod: bool=?, 'variables) => Js.Promise.t(Types.operationResult('data))` | A function for imperatively re-executing the mutation. Accepts labelled arguments for modifying specific conditions of the mutation execution.                                                                                                                        |

### Example

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

[@react.component]
let make = (~id) => {
  let (_, executeMutation) = Hooks.useMutation(~mutation=(module LikeDog));

  <button onClick={_e => executeMutation({key: id}) |> ignore}>
    "Like This Dog!"->React.string
  </button>
}
```

Check out `examples/3-mutation` to see an example of using the `useMutation` hook.

## `useSubscription`

`useSubscription` allows you to execute a GraphQL subscription. You can accumulate the results of executing subscriptions by passing a `handler` function to `useSubscription`.

If using the `useSubscription` hook, be sure your client is configured with the [`subscriptionExchange`](./exchanges#subscription-exchange).

### Arguments

| Argument          | Type                          | Description                                                                                                                                                                                                |
| ----------------- | ----------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `subscription`    | `(module Types.Operation)`    | The `graphql_ppx` module representing your GraphQL operation.                                                                                                                                              |
| `handler`         | `Hooks.handler`               | A variant type to allow for proper type inference of accumulated subscription data. A `handler` function allows you to accumulate subscription responses in the `data` field of the returned state record. |
| `requestPolicy`   | `Types.requestPolicy=?`       | Optional. The request policy to use to execute the query. Defaults to `"cache-first"`.                                                                                                                     |
| `pause`           | `bool=?`                      | Optional. A boolean flag instructing `useSubscription` to pause execution of the subscription.                                                                                                             |
| `fetchOptions`    | `Fetch.requestInit=?`         | Optional. The fetch options to apply on the outgoing request.                                                                                                                                              |
| `requestPolicy`   | `Types.requestPolicy=?`       | Optional. The request policy to use to execute the query. Defaults to `"cache-first"`.                                                                                                                     |
| `url`             | `string=?`                    | Optional. The GraphQL endpoint to use for the executing operation (if different from the one specified by `Client.make`).                                                                                  |
| `pollInterval`    | `int=?`                       | Optional. Instructs the client to reexecute the subscription every `pollInterval` milliseconds.                                                                                                            |
| `meta`            | `Types.operationDebugMeta=?`  | Optional. Add metadata that is only available in development with devtools.                                                                                                                                |
| `suspense`        | `bool=?`                      | Optional. A flag activating the experimental React suspense mode, which can be used during server-side rendering to prefetch data. Defaults to `false`.                                                    |
| `preferGetMethod` | `bool=?`                      | Optional. If `true`, will use the HTTP GET method rather than POST for operations of type `query`. Defaults to `false`.                                                                                    |
| `variables`       | `Types.Operation.t_variables` | Optional. Variables to pass as part of your GraphQL query. This should be passed as the last positional argument, if needed.                                                                               |

### Return Type

`useSubscription` returns a tuple containing the result of executing your GraphQL subscription and a function for re-executing the subscription imperatively.

| Return Value          | Type                                                                                                                                                                                                              | Description                                                                                                                                                                                                                                                           |
| --------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `result`              | `Hooks.hookResponse('response)`                                                                                                                                                                                   | A record containing fields for `fetching`, `data`, `error`, `response`, `extensions`, and `stale`. `response` is a variant containing constructors for `Data`, `PartialData`, `Error`, `Fetching`, and `Empty`. Useful for pattern matching to render conditional UI. |
| `executeSubscription` | `(~fetchOptions: Fetch.requestInit=?, ~requestPolicy: Types.requestPolicy=?, ~url: string=?, ~pollInterval: int=?, ~meta: Types.operationDebugMeta=?, ~suspense: bool=?, ~preferGetMethod: bool=?, unit) => unit` | A function for imperatively re-executing the subscription. Accepts labelled arguments for modifying specific conditions of the subscription execution.                                                                                                                |

### Example

```reason
open ReasonUrql;

module SubscribeRandomInt = [%graphql
  {|
  subscription subscribeNumbers {
    newNumber
  }
|}
];

/* Accumulate subscriptions as new values arrive from your GraphQL endpoint. */
let handler = (prevSubscriptions, subscription) => {
  switch (prevSubscriptions) {
  | Some(subs) => Array.append(subs, [|subscription|])
  | None => [|subscription|]
  };
};

[@react.component]
let make = () => {
  let (Hooks.{response}) =
    Hooks.useSubscription(
      ~request=(module SubscribeRandomInt),
      ~handler=Handler(handler),
      ()
    );

  switch (response) {
    | Fetching => <text> "Loading"->React.string </text>
    | Data(d)
    | PartialData(d, _e) =>
      d
      |> Array.map(
      (datum) =>
        <circle
          cx=datum.newNumber->string_of_int
          cy=datum.newNumber->string_of_int
          stroke="#222"
          fill="none"
          r="5"
        />,
      )
      |> React.array
    | Error(_e) => <text> "Error"->React.string </text>
    | Empty => <text> "Empty"->React.string </text>
  }
}
```

Check out `examples/5-subscription` to see an example of using the `useSubscription` hook.

## `useClient`

`useClient` allows you to access your `urql` client instance anywhere within your React tree.

### Return Type

| Return Value | Type       | Description                                      |
| ------------ | ---------- | ------------------------------------------------ |
| `client`     | `Client.t` | The `urql` client instance for your application. |

### Example

```reason
open ReasonUrql;

module GetAllDogs = [%graphql {|
  query dogs {
    dogs {
      name
      breed
      likes
    }
  }
|}];

[@react.component]
let make = () => {
  let client = Hooks.useClient();

  React.useEffect1(() => {
    let subscription = Client.executeQuery(~client, ~query=(module GetDogs), ())
      |> Wonka.subscribe((. result) => Js.log(result));

    Some(subscription.unsubscribe);
  }, [|client|]);

  <div> "You can now use your client!"->React.string </div>
}
```
