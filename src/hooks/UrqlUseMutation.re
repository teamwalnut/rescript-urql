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
  option(Js.Json.t) => Js.Promise.t(UrqlTypes.operationResult);

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
    | (true, _, _) => UrqlTypes.Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

let useMutation = (~request: UrqlTypes.request('response)) => {
  let (useMutationResponseJs, executeMutation) =
    useMutationJs(request##query);
  let useMutationResponse =
    useMutationResponseJs |> useMutationResponseToRecord(request##parse);
  (useMutationResponse, () => executeMutation(Some(request##variables)));
};
