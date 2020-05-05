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
type partialOperationContext = {
  fetchOptions: option(Fetch.requestInit),
  requestPolicy: option(UrqlTypes.requestPolicy),
  url: option(string),
  debugMeta: option(operationDebugMeta),
  pollInterval: option(int),
};

type partialOperationContextJs = {
  fetchOptions: option(Fetch.requestInit),
  requestPolicy: option(string),
  url: option(string),
  debugMeta: option(operationDebugMeta),
  pollInterval: option(int),
};

let decodeOperationRequestPolicy =
    (opts: option(partialOperationContext))
    : option(partialOperationContextJs) => {
  switch (opts) {
  | Some(o) =>
    switch (o.requestPolicy) {
    | Some(rp) =>
      Some({
        fetchOptions: o.fetchOptions,
        url: o.url,
        debugMeta: o.debugMeta,
        pollInterval: o.pollInterval,
        requestPolicy: Some(rp->UrqlTypes.requestPolicyToJs),
      })
    | None =>
      Some({
        fetchOptions: o.fetchOptions,
        url: o.url,
        debugMeta: o.debugMeta,
        pollInterval: o.pollInterval,
        requestPolicy: None,
      })
    }
  | None => None
  };
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
  | NotFound;

type clientResponse('response) = {
  data: option('response),
  error: option(UrqlCombinedError.t),
  response: response('response),
};
