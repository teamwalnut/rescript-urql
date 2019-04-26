[@bs.module "urql"] external queryComponent: ReasonReact.reactClass = "Query";

type queryRenderPropsJs('a) = {
  .
  "fetching": bool,
  "data": Js.Nullable.t('a),
  "error": Js.Nullable.t(UrqlCombinedError.t),
  "executeQuery": Js.Json.t => Js.Promise.t(UrqlTypes.operationResult),
};

type queryRenderProps('a) = {
  fetching: bool,
  data: option('a),
  error: option(UrqlCombinedError.t),
  executeQuery: Js.Json.t => Js.Promise.t(UrqlTypes.operationResult),
  response: UrqlTypes.response('a),
};

let urqlDataToRecord = (result: queryRenderPropsJs('a)) => {
  let data = result##data |> Js.Nullable.toOption;
  let error = result##error |> Js.Nullable.toOption;

  let response: UrqlTypes.response('a) =
    switch (result##fetching, data, error) {
    | (true, _, _) => Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {
    fetching: result##fetching,
    data,
    error,
    executeQuery: result##executeQuery,
    response,
  };
};

[@bs.deriving abstract]
type jsProps = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
  requestPolicy: string,
};

let make = (~query, ~variables, ~requestPolicy=`CacheFirst, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=queryComponent,
    ~props=
      jsProps(
        ~query,
        ~variables,
        ~requestPolicy=UrqlTypes.requestPolicyToJs(requestPolicy),
      ),
    result =>
    result |> urqlDataToRecord |> children
  );