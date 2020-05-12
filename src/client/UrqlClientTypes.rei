type t;

/* The result of executing a GraphQL request.
   Consists of optional data and errors fields. */
type executionResult = {
  errors: option(array(UrqlCombinedError.graphQLError)),
  data: option(Js.Json.t),
  extensions: Js.Json.t,
};

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
  fetchOptions: option(Fetch.requestInit),
  requestPolicy: UrqlTypes.requestPolicy,
  url: string,
  meta: option(operationDebugMeta),
  pollInterval: option(int),
};

/* A partial operation context, which can be passed as the second
   optional argument to executeQuery, executeMutation, and executeSubscription.
   Simply makes all properties on operationContext optional. */
[@bs.deriving {abstract: light}]
type partialOperationContext = {
  [@bs.optional]
  fetchOptions: Fetch.requestInit,
  [@bs.optional]
  requestPolicy: UrqlTypes.requestPolicy,
  [@bs.optional]
  url: string,
  [@bs.optional]
  debugMeta: operationDebugMeta,
  [@bs.optional]
  pollInterval: int,
};

module PartialOperationContextJs: {
  [@bs.deriving {abstract: light}]
  type t = {
    [@bs.optional]
    fetchOptions: Fetch.requestInit,
    [@bs.optional]
    requestPolicy: string,
    [@bs.optional]
    url: string,
    [@bs.optional]
    debugMeta: operationDebugMeta,
    [@bs.optional]
    pollInterval: int,
  };
};

let decodePartialOperationContext:
  option(partialOperationContext) => option(PartialOperationContextJs.t);

let createOperationContext:
  (
    ~fetchOptions: Fetch.requestInit=?,
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~url: string=?,
    ~debugMeta: operationDebugMeta=?,
    ~pollInterval: int=?,
    unit
  ) =>
  partialOperationContext;

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
  error: option(UrqlCombinedError.combinedErrorJs),
};

/* The record representing the response returned by the client after
   it has been converted by urqlClientResponseToReason. */
type response('response) =
  | Data('response)
  | Error(UrqlCombinedError.t)
  | NotFound;

type clientResponse('response) = {
  data: option('response),
  error: option(UrqlCombinedError.t),
  response: response('response),
};
