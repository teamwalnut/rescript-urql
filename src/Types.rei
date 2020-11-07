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
type operationResultJs('dataJs) = {
  operation,
  data: Js.Nullable.t('dataJs),
  error: option(CombinedError.combinedErrorJs),
  extensions: option(Js.Dict.t(string)),
  stale: option(bool),
};

/* The result of the GraphQL operation. */
type operationResult('data) = {
  data: option('data),
  error: option(CombinedError.t),
  extensions: option(Js.Dict.t(string)),
  response: operationResponse('data),
  stale: option(bool),
}
and operationResponse('data) =
  | Data('data)
  | Error(CombinedError.t)
  | Empty;

let operationResultToReason:
  (~response: operationResultJs('dataJs), ~parse: 'dataJs => 'data) =>
  operationResult('data);

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

/* The signature of a graphql-ppx module. */
module type Operation = {
  module Raw: {
    type t;
    type t_variables;
  };
  type t;
  type t_variables;

  let query: string;
  let parse: Raw.t => t;
  let serializeVariables: t_variables => Raw.t_variables;
  let variablesToJson: Raw.t_variables => Js.Json.t;
};

/* The response variant wraps the parsed result of executing a GraphQL operation. */
type response('data) =
  | Fetching
  | Data('data)
  | PartialData('data, array(GraphQLError.t))
  | Error(CombinedError.t)
  | Empty;

type hookResponse('data) = {
  operation,
  fetching: bool,
  data: option('data),
  error: option(CombinedError.t),
  response: response('data),
  extensions: option(Js.Json.t),
  stale: bool,
};

type hookResponseJs('dataJs) = {
  operation,
  fetching: bool,
  data: Js.Nullable.t('dataJs),
  error: option(CombinedError.combinedErrorJs),
  extensions: option(Js.Json.t),
  stale: bool,
};

let urqlResponseToReason:
  (~response: hookResponseJs('dataJs), ~parse: 'dataJs => 'data) =>
  hookResponse('data);

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
};
