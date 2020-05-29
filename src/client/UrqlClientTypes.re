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
[@bs.deriving jsConverter]
type operationType = [
  | [@bs.as "query"] `Query
  | [@bs.as "mutation"] `Mutation
  | [@bs.as "subscription"] `Subscription
  | [@bs.as "teardown"] `Teardown
];

/* Cache outcomes for operations. */
[@bs.deriving jsConverter]
type cacheOutcome = [
  | [@bs.as "miss"] `Miss
  | [@bs.as "partial"] `Partial
  | [@bs.as "hit"] `Hit
];

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

module PartialOperationContextJs = {
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

let decodePartialOperationContext = opts => {
  switch (opts) {
  | Some(o) =>
    let ctx =
      PartialOperationContextJs.t(
        ~fetchOptions=?o->fetchOptions,
        ~requestPolicy=?
          o->requestPolicy->Belt.Option.map(UrqlTypes.requestPolicyToJs),
        ~url=?o->url,
        ~debugMeta=?o->debugMeta,
        ~pollInterval=?o->pollInterval,
        (),
      );
    Some(ctx);
  | None => None
  };
};

let createOperationContext =
    (
      ~fetchOptions=?,
      ~requestPolicy=?,
      ~url=?,
      ~debugMeta=?,
      ~pollInterval=?,
      (),
    ) => {
  partialOperationContext(
    ~fetchOptions?,
    ~requestPolicy?,
    ~url?,
    ~debugMeta?,
    ~pollInterval?,
    (),
  );
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
  error: option(UrqlCombinedError.combinedErrorJs),
};

/* The record representing the response returned by the client after
   it has been converted by urqlClientResponseToReason. */
type response('response) =
  | Data('response)
  | Error(UrqlCombinedError.t)
  | Empty;

type clientResponse('response) = {
  data: option('response),
  error: option(UrqlCombinedError.t),
  response: response('response),
};
