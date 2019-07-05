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

/* The result of executing a GraphQL request.
   Consists of optional data and errors fields. */
[@bs.deriving abstract]
type executionResult = {
  [@bs.optional]
  errors: array(UrqlCombinedError.graphqlError),
  [@bs.optional]
  data: Js.Json.t,
};

/* The response variant passed to Query, Mutation, and Subscription. */
type response('a) =
  | Fetching
  | Data('a)
  | Error(UrqlCombinedError.t)
  | NotFound;

/* OperationType for the active operation.
   Use with operationTypeToJs for proper conversion to string. */
[@bs.deriving jsConverter]
type operationType = [
  | [@bs.as "subscription"] `Subscription
  | [@bs.as "query"] `Query
  | [@bs.as "mutation"] `Mutation
  | [@bs.as "teardown"] `Teardown
];

/* Additional operation metadata to pass to the active operation.
   Currently does not support additional untyped options. */
[@bs.deriving abstract]
type operationContext = {
  [@bs.optional]
  fetchOptions: Fetch.requestInit,
  requestPolicy,
  url: string,
};

/* A partial operation context, which can be passed as the second optional argument
   to executeQuery, executeMutation, and executeSubscription. */
[@bs.deriving abstract]
type partialOperationContext = {
  [@bs.optional]
  fetchOptions: Fetch.requestInit,
  [@bs.optional]
  requestPolicy,
  [@bs.optional]
  url: string,
};

/* The active GraphQL operation. */
[@bs.deriving abstract]
type operation = {
  [@bs.optional]
  variables: Js.Json.t,
  key: int,
  query: string,
  operationName: operationType,
  context: operationContext,
};

[@bs.deriving abstract]
type operationResult = {
  operation,
  [@bs.optional]
  data: Js.Json.t,
  [@bs.optional]
  error: UrqlCombinedError.t,
};

/* The signature of the Js.t created by calling `.make()` on a `graphql_ppx` module. */
type request('response) = {
  .
  "parse": Js.Json.t => 'response,
  "query": string,
  "variables": Js.Json.t,
};

type hookResponse('ret) = {
  fetching: bool,
  data: option('ret),
  error: option(UrqlCombinedError.t),
  response: response('ret),
};
