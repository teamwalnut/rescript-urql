/* RequestPolicy to be used for queries. */
[@bs.deriving jsConverter]
type requestPolicy = [
  | [@bs.as "cache-first"] `CacheFirst
  | [@bs.as "cache-only"] `CacheOnly
  | [@bs.as "network-only"] `NetworkOnly
  | [@bs.as "cache-and-network"] `CacheAndNetwork
];

/* The GraphQL request object.
   Consists of a query, a unique key for the event, and, optionally, variables. */
type graphqlRequest = {
  key: int,
  query: string,
  variables: option(Js.Json.t),
};

/* The signature of the Js.t created by calling `.make()` on a `graphql_ppx` module. */
type request('response) = {
  .
  "parse": Js.Json.t => 'response,
  "query": string,
  "variables": Js.Json.t,
};

/* The response variant wraps the parsed result of executing a GraphQL operation. */
type response('response) =
  | Fetching
  | Data('response)
  | Error(UrqlCombinedError.t)
  | NotFound;

type hookResponse('response, 'extensions) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.t),
  response: response('response),
  extensions: option('extensions),
};

type jsHookResponse('response, 'extensions) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.combinedErrorJs),
  extensions: option('extensions),
};

type graphqlDefinition('parseResult, 'composeReturnType, 'hookReturnType) = (
  // `parse`
  Js.Json.t => 'parseResult,
  // `query`
  string,
  // `composeVariables`
  (Js.Json.t => 'composeReturnType) => 'hookReturnType,
);
