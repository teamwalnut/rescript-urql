## `CombinedError`

Errors in `rescript-urql` are handed to your hooks via a record of type `CombinedError.t`. The record has the following type:

```rescript
type t = {
  name: string,
  message: string,
  graphQLErrors: array<GraphQLError.t>,
  networkError: option<Js.Exn.t>,
  response: option<Fetch.response>,
  toString: unit => string,
}
```

In this case, `networkError` returns the original JavaScript error thrown if a network error was encountered. `graphQLErrors` represent an `array` of errors of type `graphQLError`. These represent the errors encountered in the validation or execution stages of interacting with your GraphQL API. `response` is the raw `response` object returned by `fetch`. `message` is a stringified version of either the `networkError` or the `graphQLErrors` – `networkError` will take precedence.

#### Example

The easiest way to interact with `CombinedError` is through the `message` field on the `CombinedError.t` record. If you just want to display the error message to your user, or operate directly on the error string, simply do the following:

```rescript
switch response {
  | Error(e) => <div> e.message->React.string </div>
  | _ => <div />
}
```

Depending on the types of errors you get from your GraphQL API, you may want to do different things. Here's an example showing how to handle `networkError`s and `graphQLErrors` indepedently.

```rescript
let ({ response }, _) = useQuery(~query=(module MyQuery), ())

switch response {
  | Error(e) =>
    switch e {
    | {networkError: Some(ne)} =>
      <div>
        {ne
        ->Js.Exn.message
        ->Belt.Option.getWithDefault("Network error")
        ->React.string}
      </div>
    | {graphQLErrors: gqle} when gqle->Belt.Array.length > 0 =>
      <div>
        {gqle
          |> Array.to_list
          |> List.map((e: CombinedError.graphQLError) => {
              "[GraphQLError: " ++ e.message ++ "]"
            })
          |> String.concat(", ")
          |> React.string}
      </div>
    | _ => <div> "Unknown error."->React.string </div>
    }
  }
  | _ => <div />
}
```
