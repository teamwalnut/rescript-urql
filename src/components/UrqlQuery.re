[@bs.deriving abstract]
type queryRenderPropsJs = {
  fetching: bool,
  data: Js.Nullable.t(Js.Json.t),
  error: Js.Nullable.t(UrqlCombinedError.combinedErrorJs),
  executeQuery:
    option(Js.Json.t) => Js.Promise.t(UrqlClient.ClientTypes.operationResult),
};

type queryRenderProps('response) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.combinedError),
  executeQuery:
    option(Js.Json.t) => Js.Promise.t(UrqlClient.ClientTypes.operationResult),
  response: UrqlTypes.response('response),
};

module QueryJs = {
  [@bs.module "urql"] [@react.component]
  external make:
    (
      ~query: string,
      ~variables: Js.Json.t,
      ~requestPolicy: string,
      ~pause: bool=?,
      ~children: queryRenderPropsJs => React.element
    ) =>
    React.element =
    "Query";
};

let urqlQueryResponseToReason =
    (parse: Js.Json.t => 'response, result: queryRenderPropsJs)
    : queryRenderProps('response) => {
  let data = result->dataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->errorGet
    ->Js.Nullable.toOption
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->fetchingGet;
  let executeQuery = result->executeQueryGet;

  let response =
    switch (fetching, data, error) {
    | (true, _, _) => UrqlTypes.Fetching
    | (false, _, Some(error)) => Error(error)
    | (false, Some(data), _) => Data(data)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, executeQuery, response};
};

[@react.component]
let make =
    (
      ~request: UrqlTypes.request('response),
      ~requestPolicy: UrqlTypes.requestPolicy=`CacheFirst,
      ~pause: option(bool)=?,
      ~children: queryRenderProps('response) => React.element,
    ) => {
  let query = request##query;
  let variables = request##variables;
  let parse = request##parse;
  <QueryJs
    query
    variables
    requestPolicy={UrqlTypes.requestPolicyToJs(requestPolicy)}
    ?pause>
    {result => result |> urqlQueryResponseToReason(parse) |> children}
  </QueryJs>;
};
