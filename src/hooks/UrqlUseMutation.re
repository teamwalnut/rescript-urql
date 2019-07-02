open UrqlTypes;

[@bs.deriving abstract]
type useMutationResponseJs = {
  fetching: bool,
  [@bs.optional]
  data: Js.Json.t,
  [@bs.optional]
  error: UrqlCombinedError.t,
};

type executeMutation =
  option(Js.Json.t) => Js.Promise.t(operationResult);

[@bs.module "urql"]
external useMutationJs: string => (useMutationResponseJs, executeMutation) =
  "useMutation";

let useMutationResponseToRecord =
    (parse: Js.Json.t => 'response, result: useMutationResponseJs) => {
  let data = result->dataGet->Belt.Option.map(parse);
  let error = result->errorGet;
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
  let useMutationResponse = React.useMemo1(
    () => useMutationResponseJs |> useMutationResponseToRecord(request##parse),
    [|useMutationResponseJs|]
    );
  let executeMutation = React.useCallback1(() => executeMutationJs(Some(request##variables)), [|request##variables|]);
  (useMutationResponse, executeMutation);
};
