[@bs.deriving abstract]
type mutationRenderPropsJs = {
  fetching: bool,
  [@bs.optional]
  data: Js.Json.t,
  [@bs.optional]
  error: UrqlCombinedError.t,
  executeMutation:
    option(Js.Json.t) => Js.Promise.t(UrqlTypes.operationResult),
};

type mutationRenderProps('response) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.t),
  executeMutation: unit => Js.Promise.t(UrqlTypes.operationResult),
  response: UrqlTypes.response('response),
};

module MutationJs = {
  [@bs.module "urql"] [@react.component]
  external make:
    (~query: string, ~children: mutationRenderPropsJs => React.element) =>
    React.element =
    "Mutation";
};

let urqlDataToRecord = (parse, variables, result) => {
  let data = result->dataGet->Belt.Option.map(parse);
  let error = result->errorGet;
  let fetching = result->fetchingGet;
  let executeMutationFn = result->executeMutationGet;
  let executeMutation = () => executeMutationFn(Some(variables));

  let response: UrqlTypes.response('a) =
    switch (fetching, data, error) {
    | (true, _, _) => Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, executeMutation, response};
};

[@react.component]
let make =
    (
      ~request: UrqlTypes.request('response),
      ~children: mutationRenderProps('response) => React.element,
    ) => {
  let query = request##query;
  let variables = request##variables;
  let parse = request##parse;
  <MutationJs query>
    {result => result |> urqlDataToRecord(parse, variables) |> children}
  </MutationJs>;
};
