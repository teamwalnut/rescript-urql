open UrqlTypes;

[@bs.deriving abstract]
type useQueryArgs = {
  query: string,
  variables: Js.Json.t,
  [@bs.optional]
  requestPolicy,
  [@bs.optional]
  pause: bool,
};

type partialOperationContextFn =
  option(UrqlClient.Types.partialOperationContext) => unit;
type useQueryResponseJs = (jsResponse, partialOperationContextFn);

type useQueryResponse('response) = (
  hookResponse('response),
  partialOperationContextFn,
);

[@bs.module "urql"]
external useQueryJs: useQueryArgs => useQueryResponseJs = "useQuery";

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
  let state_record =
    React.useMemo2(
      () => state |> urqlResponseToReason(request##parse),
      (state, request##parse),
    );

  (state_record, executeQuery);
};
