[@bs.deriving abstract]
type queryRenderPropsJs = {
  fetching: bool,
  data: Js.Nullable.t(Js.Json.t),
  [@bs.optional]
  error: UrqlCombinedError.t,
  executeQuery: option(Js.Json.t) => Js.Promise.t(UrqlTypes.operationResult),
};

type queryRenderProps('response) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.t),
  executeQuery: option(Js.Json.t) => Js.Promise.t(UrqlTypes.operationResult),
  response: UrqlTypes.response('response),
};

module QueryJs = {
  [@bs.module "urql"] [@react.component]
  external make:
    (
      ~query: string,
      ~variables: Js.Json.t,
      ~requestPolicy: string,
      ~pause: option(bool)=?,
      ~children: queryRenderPropsJs => React.element
    ) =>
    React.element =
    "Query";
};

let urqlDataToRecord = (parse, result) => {
  let data = result->dataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error = result->errorGet;
  let fetching = result->fetchingGet;
  let executeQuery = result->executeQueryGet;

  let response =
    switch (fetching, data, error) {
    | (true, _, _) => UrqlTypes.Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
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
    pause>
    {result => result |> urqlDataToRecord(parse) |> children}
  </QueryJs>;
};
