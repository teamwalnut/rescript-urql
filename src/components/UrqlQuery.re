[@bs.deriving abstract]
type queryRenderPropsJs('extensions) = {
  fetching: bool,
  data: Js.Nullable.t(Js.Json.t),
  error: Js.Nullable.t(UrqlCombinedError.combinedErrorJs),
  executeQuery:
    option(UrqlClient.ClientTypes.partialOperationContext) => unit,
  extensions: Js.Nullable.t('extensions),
};

type queryRenderProps('response, 'extensions) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.t),
  executeQuery:
    option(UrqlClient.ClientTypes.partialOperationContext) => unit,
  response: UrqlTypes.response('response),
  extensions: option('extensions),
};

module QueryJs = {
  [@bs.module "urql"] [@react.component]
  external make:
    (
      ~query: string,
      ~variables: Js.Json.t,
      ~requestPolicy: string,
      ~pause: bool=?,
      ~context: UrqlClient.ClientTypes.partialOperationContext=?,
      ~children: queryRenderPropsJs('extensions) => React.element
    ) =>
    React.element =
    "Query";
};

let urqlQueryResponseToReason =
    (parse: Js.Json.t => 'response, result: queryRenderPropsJs('extensions))
    : queryRenderProps('response, 'extensions) => {
  let data = result->dataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->errorGet
    ->Js.Nullable.toOption
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->fetchingGet;
  let executeQuery = result->executeQueryGet;
  let extensions = result->extensionsGet->Js.Nullable.toOption;

  let response =
    switch (fetching, data, error) {
    | (true, _, _) => UrqlTypes.Fetching
    | (false, _, Some(error)) => Error(error)
    | (false, Some(data), _) => Data(data)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, executeQuery, response, extensions};
};

[@react.component]
let make =
    (
      ~request: UrqlTypes.request('response),
      ~requestPolicy: UrqlTypes.requestPolicy=`CacheFirst,
      ~pause: option(bool)=?,
      ~context: option(UrqlClient.ClientTypes.partialOperationContext)=?,
      ~children: queryRenderProps('response, 'extension) => React.element,
    ) => {
  let query = request##query;
  let variables = request##variables;
  let parse = request##parse;
  <QueryJs
    query
    variables
    requestPolicy={UrqlTypes.requestPolicyToJs(requestPolicy)}
    ?pause
    ?context>
    {result => result |> urqlQueryResponseToReason(parse) |> children}
  </QueryJs>;
};
