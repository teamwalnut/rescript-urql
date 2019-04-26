[@bs.module "urql"]
external mutationComponent: ReasonReact.reactClass = "Mutation";

type mutationRenderPropsJs('a) = {
  .
  "fetching": bool,
  "data": Js.Nullable.t('a),
  "error": Js.Nullable.t(UrqlCombinedError.t),
  "executeMutation": Js.Json.t => Js.Promise.t(UrqlTypes.operationResult),
};

type mutationRenderProps('a) = {
  fetching: bool,
  data: option('a),
  error: option(UrqlCombinedError.t),
  executeMutation: Js.Json.t => Js.Promise.t(UrqlTypes.operationResult),
  response: UrqlTypes.response('a),
};

let urqlDataToRecord = (result: mutationRenderPropsJs('a)) => {
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
    executeMutation: result##executeMutation,
    response,
  };
};

[@bs.deriving abstract]
type jsProps = {query: string};

let make =
    (
      ~query: string,
      children: mutationRenderProps('a) => ReasonReact.reactElement,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=mutationComponent, ~props=jsProps(~query), result =>
    result |> urqlDataToRecord |> children
  );