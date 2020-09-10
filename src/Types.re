/* RequestPolicy to be used for queries. */
[@bs.deriving jsConverter]
type requestPolicy = [
  | [@bs.as "cache-first"] `CacheFirst
  | [@bs.as "cache-only"] `CacheOnly
  | [@bs.as "network-only"] `NetworkOnly
  | [@bs.as "cache-and-network"] `CacheAndNetwork
];

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
  data: Js.Nullable.t(Js.Json.t),
  error: option(CombinedError.combinedErrorJs),
  extensions: option(Js.Dict.t(string)),
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

type hookResponse('response) = {
  operation,
  fetching: bool,
  data: option('response),
  error: option(CombinedError.t),
  response: response('response),
  extensions: option(Js.Json.t),
  stale: bool,
};

type hookResponseJs('response) = {
  operation,
  fetching: bool,
  data: Js.Nullable.t('response),
  error: option(CombinedError.combinedErrorJs),
  extensions: option(Js.Json.t),
  stale: bool,
};

/**
 * A function for converting the response to an urql hook from its
 * JavaScript representation to a typed Reason record.
 */
let urqlResponseToReason = (~response, ~parse) => {
  let {operation, fetching, extensions, stale} = response;

  let data = response.data->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    response.error->Belt.Option.map(CombinedError.combinedErrorToRecord);

  let response =
    switch (fetching, data, error) {
    | (true, None, _) => Fetching
    | (_, Some(d), None) => Data(d)
    | (_, Some(d), Some(e)) => PartialData(d, e.graphQLErrors)
    | (false, _, Some(e)) => Error(e)
    | (false, None, None) => Empty
    };

  {operation, fetching, data, error, response, extensions, stale};
};

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
