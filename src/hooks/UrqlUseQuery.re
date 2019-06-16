[@bs.deriving abstract]
type useQueryStateJs('a) = {
  fetching: bool,
  [@bs.optional] data: 'a,
  [@bs.optional] error: UrqlCombinedError.t
};

[@bs.deriving abstract]
type useQueryArgs('a) = {
  query: string,
  [@bs.optional] variables: 'a,
  [@bs.optional] requestPolicy: UrqlTypes.requestPolicy,
  [@bs.optional] pause: bool
}

type partialOperationContextFn = option(UrqlTypes.partialOperationContext) => unit;
type useQueryResponseJs('a) = (useQueryStateJs('a), partialOperationContextFn);

type useQueryState('a) = {
  fetching: bool,
  data: option('a),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('a)
}
type useQueryResponse('a) = (useQueryState('a), partialOperationContextFn);

let useQueryResponseToRecord =
    (parse: Js.Json.t => 'response, result: useQueryStateJs('a)) => {
  let data =
    switch (result->dataGet) {
    | None => None
    | Some(d) => Some(parse(d))
    };
  let error = result->errorGet;
  let fetching = result->fetchingGet;

  let response: UrqlTypes.response('response) =
    switch (fetching, data, error) {
    | (true, _, _) => Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

[@bs.module "urql"]
external useQueryJs: (useQueryArgs('a)) => useQueryResponseJs('b) = "useQuery";

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

  (state_record, executeQuery)
}
