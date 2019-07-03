open UrqlTypes;

[@bs.deriving abstract]
type useQueryStateJs = {
  fetching: bool,
  data: Js.Nullable.t(Js.Json.t),
  [@bs.optional]
  error: UrqlCombinedError.t,
};

[@bs.deriving abstract]
type useQueryArgs = {
  query: string,
  variables: Js.Json.t,
  [@bs.optional]
  requestPolicy: requestPolicy,
  [@bs.optional]
  pause: bool,
};

type partialOperationContextFn =
  option(partialOperationContext) => unit;
type useQueryResponseJs = (useQueryStateJs, partialOperationContextFn);

type useQueryResponse('response) = (
  hookResponse('response),
  partialOperationContextFn,
);

let useQueryResponseToRecord = (parse, result) => {
  let data = result->dataGet->Js.Nullable.toOption->Belt.Option.map(parse);
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

[@bs.module "urql"]
external useQueryJs: useQueryArgs => useQueryResponseJs = "useQuery";

let useQuery = (~request, ~requestPolicy=?, ~pause=?, ()) => {
  let args =
    useQueryArgs(
      ~query=request##query,
      ~variables=request##variables,
      ~requestPolicy?,
      ~pause?,
      (),
    );
  let (state, executeQuery) = useQueryJs(args);
  let state_record = state |> useQueryResponseToRecord(request##parse);

  (state_record, executeQuery);
};
