/* RequestPolicy to be used for queries. */
@deriving(jsConverter)
type requestPolicy = [
  | #"cache-first"
  | #"cache-only"
  | #"network-only"
  | #"cache-and-network"
]

/* OperationType for the active operation.
 Use with operationTypeToJs for proper conversion to strings. */
@deriving(jsConverter)
type operationType = [
  | @as("query") #Query
  | @as("mutation") #Mutation
  | @as("subscription") #Subscription
  | @as("teardown") #Teardown
]

/* Cache outcomes for operations. */
@deriving(jsConverter)
type cacheOutcome = [@as("miss") #Miss | @as("partial") #Partial | @as("hit") #Hit]

/* Debug information on operations. */
type operationDebugMeta = {
  source: option<string>,
  cacheOutcome: option<cacheOutcome>,
  networkLatency: option<int>,
  startTime: option<int>,
}

/* The operation context object for a request. */
type operationContext = {
  additionalTypenames: option<array<string>>,
  fetch: option<(string, Fetch.Request.init) => Js.Promise.t<Fetch.Response.t>>,
  fetchOptions: option<Fetch.Request.init>,
  requestPolicy: requestPolicy,
  url: string,
  meta: option<operationDebugMeta>,
  suspense: option<bool>,
  preferGetMethod: option<bool>,
}

@deriving({abstract: light})
type partialOperationContext = {
  @optional
  additionalTypenames: array<string>,
  @optional
  fetch: (string, Fetch.Request.init) => Js.Promise.t<Fetch.Response.t>,
  @optional
  fetchOptions: Fetch.Request.init,
  @optional
  requestPolicy: string,
  @optional
  url: string,
  @optional
  meta: operationDebugMeta,
  @optional
  suspense: bool,
  @optional
  preferGetMethod: bool,
}

/* The active GraphQL operation. */
type operation = {
  key: int,
  query: string,
  variables: option<Js.Json.t>,
  kind: operationType,
  context: operationContext,
}

/* The result of the GraphQL operation. */
type operationResultJs<'dataJs> = {
  operation: operation,
  data: Js.Nullable.t<'dataJs>,
  error: option<CombinedError.t>,
  extensions: option<Js.Dict.t<string>>,
  stale: option<bool>,
}

type operationResponse<'data> =
  | Data('data)
  | Error(CombinedError.t)
  | Empty

/* The result of the GraphQL operation. */
type operationResult<'data> = {
  data: option<'data>,
  error: option<CombinedError.t>,
  extensions: option<Js.Dict.t<string>>,
  response: operationResponse<'data>,
  stale: option<bool>,
}

let operationResultToReScript = (
  ~response: operationResultJs<'dataJs>,
  ~parse: 'dataJs => 'data,
) => {
  let {error, extensions, stale}: operationResultJs<'dataJs> = response
  let data = response.data->Js.Nullable.toOption->Belt.Option.map(parse)

  let response = switch (data, error) {
  | (Some(data), _) => Data(data)
  | (None, Some(error)) => Error(error)
  | (None, None) => Empty
  }

  {data, error, extensions, stale, response}
}

/* The GraphQL request object.
 Consists of a query, a unique key for the event, and, optionally, variables. */
type graphqlRequest = {
  key: int,
  query: string,
  variables: option<Js.Json.t>,
}

/* The signature of a graphql-ppx module. */
module type Operation = {
  module Raw: {
    type t
    type t_variables
  }
  type t
  type t_variables

  let query: string
  let parse: Raw.t => t
  let serializeVariables: t_variables => Raw.t_variables
  let variablesToJson: Raw.t_variables => Js.Json.t
}

module Hooks = {
  /* The response variant wraps the parsed result of executing a GraphQL operation. */
  type response<'data> =
    | Fetching
    | Data('data)
    | PartialData('data, array<GraphQLError.t>)
    | Error(CombinedError.t)
    | Empty

  type hookResponse<'data> = {
    operation: operation,
    fetching: bool,
    data: option<'data>,
    error: option<CombinedError.t>,
    response: response<'data>,
    extensions: option<Js.Json.t>,
    stale: bool,
  }

  type hookResponseJs<'dataJs> = {
    operation: operation,
    fetching: bool,
    data: Js.Nullable.t<'dataJs>,
    error: option<CombinedError.t>,
    extensions: option<Js.Json.t>,
    stale: bool,
  }

  /*
   * A function for converting the response to an urql hook from its
   * JavaScript representation to a typed ReScript record.
   */
  let hookResponseToReScript:
    type dataJs data. (
      ~response: hookResponseJs<dataJs>,
      ~parse: dataJs => data,
    ) => hookResponse<data> =
    (~response, ~parse) => {
      let {operation, fetching, error, extensions, stale} = response

      let data = response.data->Js.Nullable.toOption->Belt.Option.map(parse)

      let response = switch (fetching, data, error) {
      | (true, None, _) => Fetching
      | (_, Some(d), None) => Data(d)
      | (_, Some(d), Some(e)) => PartialData(d, e.graphQLErrors)
      | (false, _, Some(e)) => Error(e)
      | (false, None, None) => Empty
      }

      {
        operation,
        fetching,
        data,
        error,
        response,
        extensions,
        stale,
      }
    }
}

type graphqlDefinition<'parseResult, 'composeReturnType, 'hookReturnType> = (
  // `parse`
  Js.Json.t => 'parseResult,
  // `query`
  string,
  // `composeVariables`
  (Js.Json.t => 'composeReturnType) => 'hookReturnType,
)

/* The result of executing a GraphQL request.
 Consists of optional data and errors fields. */
type executionResult = {
  errors: option<array<GraphQLError.t>>,
  data: option<Js.Json.t>,
}
