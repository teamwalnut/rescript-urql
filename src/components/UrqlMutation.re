[@bs.deriving abstract]
type mutationRenderPropsJs('a) = {
  fetching: bool,
  [@bs.optional]
  data: 'a,
  [@bs.optional]
  error: UrqlCombinedError.t,
  executeMutation:
    option(Js.Json.t) => Js.Promise.t(UrqlTypes.operationResult),
};

type mutationRenderProps('a) = {
  fetching: bool,
  data: option('a),
  error: option(UrqlCombinedError.t),
  executeMutation:
    option(Js.Json.t) => Js.Promise.t(UrqlTypes.operationResult),
  response: UrqlTypes.response('a),
};

module MutationJs = {
  [@bs.module "urql"] [@react.component]
  external make:
    (~query: string, ~children: mutationRenderPropsJs('a) => React.element) =>
    React.element =
    "Mutation";
};

let urqlDataToRecord = (result: mutationRenderPropsJs('a)) => {
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
    executeMutation: result->executeMutationGet,
    response,
  };
};

[@react.component]
let make =
    (~query: string, ~children: mutationRenderProps('a) => React.element) =>
  <MutationJs query>
    {result => result |> urqlDataToRecord |> children}
  </MutationJs>;