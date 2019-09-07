[@bs.deriving abstract]
type mutationRenderPropsJs = {
  fetching: bool,
  data: Js.Nullable.t(Js.Json.t),
  error: Js.Nullable.t(UrqlCombinedError.t),
  executeMutation:
    option(Js.Json.t) => Js.Promise.t(UrqlClient.Types.operationResult),
};

type mutationRenderProps('response) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.combinedError),
  executeMutation: unit => Js.Promise.t(UrqlClient.Types.operationResult),
  response: UrqlTypes.response('response),
};

module MutationJs = {
  [@bs.module "urql"] [@react.component]
  external make:
    (~query: string, ~children: mutationRenderPropsJs => React.element) =>
    React.element =
    "Mutation";
};

let urqlMutationResponseToReason =
    (
      parse: Js.Json.t => 'response,
      variables: Js.Json.t,
      result: mutationRenderPropsJs,
    )
    : mutationRenderProps('response) => {
  let data = result->dataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->errorGet
    ->Js.Nullable.toOption
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->fetchingGet;
  let executeMutationFn = result->executeMutationGet;
  let executeMutation = () => executeMutationFn(Some(variables));

  let response =
    switch (fetching, data, error) {
    | (true, _, _) => UrqlTypes.Fetching
    | (false, _, Some(error)) => Error(error)
    | (false, Some(data), _) => Data(data)
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
    {result =>
       result |> urqlMutationResponseToReason(parse, variables) |> children}
  </MutationJs>;
};
