/* RequestPolicy to be used for queries.
   Use with requestPolicyToJs for proper conversion to string. */
[@bs.deriving jsConverter]
type requestPolicy = [
  | [@bs.as "cache-first"] `CacheFirst
  | [@bs.as "cache-only"] `CacheOnly
  | [@bs.as "network-only"] `NetworkOnly
  | [@bs.as "cache-and-network"] `CacheAndNetwork
];

/* The GraphQL request object.
   Consists of a query, a unique key for the event, and, optionally, variables. */
[@bs.deriving abstract]
type graphqlRequest = {
  key: int,
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
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

type hookResponse('ret, 'extensions) = {
  fetching: bool,
  stale: bool,
  data: option('ret),
  error: option(UrqlCombinedError.t),
  response: response('ret),
  extensions: option('extensions),
};

[@bs.deriving abstract]
type jsResponse('response, 'extensions) = {
  fetching: bool,
  stale: bool, // TODO: can we indicate this as only for useQuery?
  [@bs.as "data"]
  jsData: Js.Nullable.t('response),
  [@bs.optional] [@bs.as "error"]
  jsError: UrqlCombinedError.combinedErrorJs,
  extensions: Js.Nullable.t('extensions),
};

type graphqlDefinition('parseResult, 'composeReturnType, 'hookReturnType) = (
  // `parse`
  Js.Json.t => 'parseResult,
  // `query`
  string,
  // `composeVariables`
  (Js.Json.t => 'composeReturnType) => 'hookReturnType,
);
