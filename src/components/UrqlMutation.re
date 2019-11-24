[@bs.deriving abstract]
type mutationRenderPropsJs('extensions) = {
  fetching: bool,
  data: Js.Nullable.t(Js.Json.t),
  error: Js.Nullable.t(UrqlCombinedError.combinedErrorJs),
  executeMutation:
    (
      option(Js.Json.t),
      option(UrqlClient.ClientTypes.partialOperationContext)
    ) =>
    Js.Promise.t(UrqlClient.ClientTypes.operationResult),
  extensions: Js.Nullable.t('extensions),
};

type mutationRenderProps('response, 'extensions) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.t),
  executeMutation:
    option(UrqlClient.ClientTypes.partialOperationContext) =>
    Js.Promise.t(UrqlClient.ClientTypes.operationResult),
  response: UrqlTypes.response('response),
  extensions: option('extensions),
};

module MutationJs = {
  [@bs.module "urql"] [@react.component]
  external make:
    (
      ~query: string,
      ~children: mutationRenderPropsJs('extensions) => React.element
    ) =>
    React.element =
    "Mutation";
};

let urqlMutationResponseToReason =
    (
      parse: Js.Json.t => 'response,
      variables: Js.Json.t,
      result: mutationRenderPropsJs('extensions),
    )
    : mutationRenderProps('response, 'extensions) => {
  let data = result->dataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->errorGet
    ->Js.Nullable.toOption
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->fetchingGet;
  let executeMutation = context =>
    result->executeMutationGet(Some(variables), context);
  let extensions = result->extensionsGet->Js.Nullable.toOption;

  let response =
    switch (fetching, data, error) {
    | (true, _, _) => UrqlTypes.Fetching
    | (false, _, Some(error)) => Error(error)
    | (false, Some(data), _) => Data(data)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, executeMutation, response, extensions};
};

[@react.component]
let make =
    (
      ~request: UrqlTypes.request('response),
      ~children: mutationRenderProps('response, 'extensions) => React.element,
    ) => {
  let query = request##query;
  let variables = request##variables;
  let parse = request##parse;
  <MutationJs query>
    {result =>
       result |> urqlMutationResponseToReason(parse, variables) |> children}
  </MutationJs>;
};
