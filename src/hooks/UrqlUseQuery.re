/* Arguments passed to useQuery on the JavaScript side. */
[@bs.deriving abstract]
type useQueryArgs = {
  query: string,
  variables: Js.Json.t,
  [@bs.optional]
  requestPolicy: UrqlTypes.requestPolicy,
  [@bs.optional]
  pause: bool,
};

/**
 * The type of executeQuery – a function for re-executing
 * the query passed to useQuery. Accepts an optional partial
 * operation context for modifying query execution.
 */
type executeQuery =
  option(UrqlClient.ClientTypes.partialOperationContext) => unit;

/* The response to useQuery on the JavaScript side. */
type useQueryResponseJs = (UrqlTypes.jsResponse, executeQuery);

/**
 * The response to useQuery – a two dimensional tuple containing
 * the result of executing the query and a function for re-executing
 * the query imperatively.
 */
type useQueryResponse('response) = (
  UrqlTypes.hookResponse('response),
  executeQuery,
);

[@bs.module "urql"]
external useQueryJs: useQueryArgs => useQueryResponseJs = "useQuery";

/**
 * A function for converting the response to useQuery from the JavaScript
 * representation to a typed Reason record.
 */
let urqlResponseToReason =
    (parse: Js.Json.t => 'response, result: UrqlTypes.jsResponse)
    : UrqlTypes.hookResponse('response) => {
  let data =
    result->UrqlTypes.jsDataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->UrqlTypes.jsErrorGet
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->UrqlTypes.fetchingGet;

  let response =
    switch (fetching, data, error) {
    | (true, _, _) => UrqlTypes.Fetching
    | (false, _, Some(error)) => Error(error)
    | (false, Some(data), _) => Data(data)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

/**
 * The useQuery hook.
 *
 * Accepts the following arguments:
 *
 * request – a Js.t containing the query and variables corresponding
 * to the GraphQL query, and a parse function for decoding the JSON response.
 *
 * requestPolicy - the request policy used to execute the query.
 * Can be one off `CacheFirst, `CacheOnly, `NetworkOnly, and `CacheAndNetwork.
 *
 * pause - prevent eager execution of the query.
 * The query will only execute when puse becomes false.
 */
let useQuery = (~request, ~requestPolicy=?, ~pause=?, ()) => {
  let args =
    useQueryArgs(
      ~query=request##query,
      ~variables=request##variables,
      ~requestPolicy?,
      ~pause?,
      (),
    );

  let (responseJs, executeQuery) = useQueryJs(args);

  let response =
    React.useMemo2(
      () => responseJs |> urqlResponseToReason(request##parse),
      (responseJs, request##parse),
    );

  (response, executeQuery);
};
