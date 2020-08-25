/* RequestPolicy to be used for queries.
   Use with requestPolicyToJs for proper conversion to string. */
[@bs.deriving jsConverter]
type requestPolicy = [
  | `CacheFirst
  | `CacheOnly
  | `NetworkOnly
  | `CacheAndNetwork
];

/* OperationType for the active operation.
   Use with operationTypeToJs for proper conversion to strings. */
type operationType = [ | `Query | `Mutation | `Subscription | `Teardown];

/* Cache outcomes for operations. */
type cacheOutcome = [ | `Miss | `Partial | `Hit];

/* Debug information on operations. */
type operationDebugMeta = {
  source: option(string),
  cacheOutcome: option(cacheOutcome),
  networkLatency: option(int),
  startTime: option(int),
};

/* The operation context object for a request. */
type operationContext = {
  additionalTypenames: option(array(string)),
  fetch: option((string, Fetch.requestInit) => Js.Promise.t(Fetch.response)),
  fetchOptions: option(Fetch.requestInit),
  requestPolicy,
  url: string,
  pollInterval: option(int),
  meta: option(operationDebugMeta),
  suspense: option(bool),
  preferGetMethod: option(bool),
};

[@bs.deriving {abstract: light}]
type partialOperationContext = {
  [@bs.optional]
  additionalTypenames: array(string),
  [@bs.optional]
  fetch: (string, Fetch.requestInit) => Js.Promise.t(Fetch.response),
  [@bs.optional]
  fetchOptions: Fetch.requestInit,
  [@bs.optional]
  requestPolicy: string,
  [@bs.optional]
  url: string,
  [@bs.optional]
  pollInterval: int,
  [@bs.optional]
  meta: operationDebugMeta,
  [@bs.optional]
  suspense: bool,
  [@bs.optional]
  preferGetMethod: bool,
};

/* The active GraphQL operation. */
type operation = {
  key: int,
  query: string,
  variables: option(Js.Json.t),
  operationName: operationType,
  context: operationContext,
};

/* The result of the GraphQL operation. */
type operationResult = {
  operation,
  data: option(Js.Json.t),
  error: option(CombinedError.combinedErrorJs),
  stale: option(bool),
};

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
  | PartialData('response, array(GraphQLError.t))
  | Error(CombinedError.t)
  | Empty;

type hookResponse('response, 'extensions) = {
  operation,
  fetching: bool,
  data: option('response),
  error: option(CombinedError.t),
  response: response('response),
  extensions: option('extensions),
  stale: bool,
};

type jsHookResponse('response, 'extensions) = {
  operation,
  fetching: bool,
  data: option('response),
  error: option(CombinedError.combinedErrorJs),
  extensions: option('extensions),
  stale: bool,
};

let urqlResponseToReason:
  (
    ~response: jsHookResponse(Js.Json.t, 'extensions),
    ~parse: Js.Json.t => 'response
  ) =>
  hookResponse('response, 'extensions);

type graphqlDefinition('parseResult, 'composeReturnType, 'hookReturnType) = (
  // `parse`
  Js.Json.t => 'parseResult,
  // `query`
  string,
  // `composeVariables`
  (Js.Json.t => 'composeReturnType) => 'hookReturnType,
);

/* The result of executing a GraphQL request.
   Consists of optional data and errors fields. */
type executionResult = {
  errors: option(array(GraphQLError.t)),
  data: option(Js.Json.t),
  extensions: Js.Json.t,
};
