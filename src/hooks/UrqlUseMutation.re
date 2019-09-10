open UrqlTypes;

type executeMutation =
  option(Js.Json.t) => Js.Promise.t(UrqlClient.Types.operationResult);

[@bs.module "urql"]
external useMutationJs: string => (jsResponse, executeMutation) =
  "useMutation";

let urqlResponseToReason = (parse: Js.Json.t => 'response, result: jsResponse) => {
  let data = result->jsDataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->jsErrorGet
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->fetchingGet;

  let response =
    switch (fetching, data, error) {
    | (true, _, _) => Fetching
    | (false, _, Some(error)) => Error(error)
    | (false, Some(data), _) => Data(data)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

let useMutation = (~request: request('response)) => {
  let (useMutationResponseJs, executeMutationJs) =
    useMutationJs(request##query);
  let useMutationResponse =
    React.useMemo2(
      () => useMutationResponseJs |> urqlResponseToReason(request##parse),
      (request##parse, useMutationResponseJs),
    );
  let executeMutation =
    React.useCallback1(
      () => executeMutationJs(Some(request##variables)),
      [|request##variables|],
    );
  (useMutationResponse, executeMutation);
};
