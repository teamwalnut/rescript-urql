[@bs.deriving abstract]
type queryRenderPropsJs('a) = {
  fetching: bool,
  [@bs.optional] data: 'a,
  [@bs.optional] error: UrqlCombinedError.t,
  executeQuery: option(Js.Json.t) => Js.Promise.t(UrqlTypes.operationResult)
};

type queryRenderProps('a) = {
  fetching: bool,
  data: option('a),
  error: option(UrqlCombinedError.t),
  executeQuery: option(Js.Json.t) => Js.Promise.t(UrqlTypes.operationResult),
  response: UrqlTypes.response('a)
};

module QueryJs = {
  [@bs.module "urql"] [@react.component]
  external make:
    (~query: string, ~variables: option(Js.Json.t)=?, ~requestPolicy: string, ~pause: option(bool)=?, ~children: queryRenderPropsJs('a) => React.element) =>
    React.element =
    "Query";
}

let urqlDataToRecord = (result: queryRenderPropsJs('a)) => {
  let data = result->dataGet;
  let error = result->errorGet;
  let fetching = result->fetchingGet;

  let response: UrqlTypes.response('a) =
    switch (fetching, data, error) {
    | (true, _, _) => Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {
    fetching,
    data,
    error,
    executeQuery: result->executeQueryGet,
    response,
  };
};

[@react.component]
let make = (
  ~query: string,
  ~variables: option(Js.Json.t)=?,
  ~requestPolicy: UrqlTypes.requestPolicy=`CacheFirst,
  ~pause: option(bool)=?,
  ~children: queryRenderProps('a) => React.element
) => {
  <QueryJs query variables requestPolicy=UrqlTypes.requestPolicyToJs(requestPolicy) pause>
    {result => result |> urqlDataToRecord |> children}
  </QueryJs>
};
