open UrqlTypes;

[@bs.deriving abstract]
type useMutationResponseJs = {
  fetching: bool,
  data: Js.Nullable.t(Js.Json.t),
  [@bs.optional]
  error: UrqlCombinedError.t,
};

type executeMutation = option(Js.Json.t) => Js.Promise.t(operationResult);

[@bs.module "urql"]
external useMutationJs: string => (useMutationResponseJs, executeMutation) =
  "useMutation";

let useMutationResponseToRecord =
    (parse: Js.Json.t => 'response, result: useMutationResponseJs) => {
  let data = result->dataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->errorGet
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->fetchingGet;

  let response =
    switch (fetching, data, error) {
    | (true, _, _) => Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

let useMutation = (~request: request('response)) => {
  let (useMutationResponseJs, executeMutationJs) =
    useMutationJs(request##query);
  let useMutationResponse =
    React.useMemo2(
      () =>
        useMutationResponseJs |> useMutationResponseToRecord(request##parse),
      (request##parse, useMutationResponseJs),
    );
  let executeMutation =
    React.useCallback1(
      () => executeMutationJs(Some(request##variables)),
      [|request##variables|],
    );
  (useMutationResponse, executeMutation);
};
