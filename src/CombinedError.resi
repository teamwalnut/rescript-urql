type t = {
  name: string,
  message: string,
  graphQLErrors: array<GraphQLError.t>,
  networkError: option<Js.Exn.t>,
  response: option<Fetch.response>,
  toString: unit => string,
}
