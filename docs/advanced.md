# Advanced

Curious to know more about the internals of `rescript-urql`? You've come to the right place!

## The `response` variant

`rescript-urql` uses a special `variant` called `response` to make pattern matching on the results you get from your GraphQL API as easy as possible. In JS land, users have to do something like the following:

```js
// Inside a React component
const [result] = useQuery({ query, variables });

if (result.fetching) {
  return <Loading />; // Some fetching UI.
} else if (result.error) {
  return <Error error={result.error} />; // Some error UI.
}

return <Data data={result.data} />; // Some data UI.
```

This is fine and works great for JS users, but we have pattern matching in ReScript! Enter `rescript-urql`'s `response` variant. `response` is a single variant with five constructors:

```reason
type response('response) {
  | Fetching
  | Data('response)
  | PartialData('response, GraphQLError.t)
  | Error(CombinedError.t)
  | Empty;
}
```

This variant is always available on the record returned by `rescript-urql`'s hooks. Let's go over what each constructor (or "tag") means and when you'd want to pattern match on each.

### The `Fetching` constructor

`Fetching` refers to when your query, mutation, or subscription is fetching data but has not yet received a response. For `useQuery` and `useMutation` this will turn `true` when the initial request has been fired off, and turn to `false` once data (or an error) has been received. Note that for `useSubscription`, `fetching` will always be `true` as long as the subcription is active. For this case, the `Data` constructor will take precedence even while `fetching` is still `true`.

### The `Data` constructor

`Data(d)` refers to when your query, mutation, or subscription has received data from your GraphQL API. It takes a single argument, `d`, that represents the actual data returned by your GraphQL API. You can then operate on `d` however you like to return UI.

### The `PartialData` constructor

`PartialData(d, e)` is quite similar to `Data` with one major difference – it represents the case where **both** data and errors were returned by your GraphQL API. This can happen if data resolves fine for parts of your query or mutation but fails to resolve for others. The type of `e` is our binding to `GraphQLError`, which gives you access to the message, source locations, and extensions of the error returned by your GraphQL server.

### The `Error` constructor

`Error(e)` refers to when your query, mutation, or subscription has encountered an error in the course of execution. It takes a single argument, `e`, that represents the [`CombinedError`](https://formidable.com/open-source/urql/docs/basics/errors/) returned by `urql`. `CombinedError` will return a record containing `message`, `networkError`, `graphQLErrors`, and `response`, allowing you to subsequently render different UI based on the _types_ of errors you encountered.

### The `Empty` constructor

The `Empty` constructor is reserved for the rare case where `rescript-urql` has made a request to your GraphQL API but neither `data` nor `errors` were returned. In practice, this case is very rare. It may occur if your user goes offline unexpectedly or if your GraphQL API returns `undefined` in cases where no data matches a particular resolver. Think of it as your fallback case.
