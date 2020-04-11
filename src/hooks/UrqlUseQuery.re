/* Arguments passed to useQuery on the JavaScript side. */
[@bs.deriving abstract]
type useQueryArgs = {
  query: string,
  variables: Js.Json.t,
  [@bs.optional]
  requestPolicy: string,
  [@bs.optional]
  pause: bool,
  [@bs.optional]
  context: UrqlClient.ClientTypes.partialOperationContextJs,
  [@bs.optional]
  pollInterval: int,
};

/**
 * The type of executeQuery – a function for re-executing
 * the query passed to useQuery. Accepts an optional partial
 * operation context for modifying query execution.
 */
type executeQueryJs =
  option(UrqlClient.ClientTypes.partialOperationContextJs) => unit;

type executeQuery =
  (~context: UrqlClient.ClientTypes.partialOperationContext=?, unit) => unit;

/* The response to useQuery on the JavaScript side. */
type useQueryResponseJs('extensions) = (
  UrqlTypes.jsResponse(Js.Json.t, 'extensions),
  executeQueryJs,
);

/**
 * The response to useQuery – a two dimensional tuple containing
 * the result of executing the query and a function for re-executing
 * the query imperatively.
 */
type useQueryResponse('response, 'extensions) = (
  UrqlTypes.hookResponse('response, 'extensions),
  executeQuery,
);

[@bs.module "urql"]
external useQueryJs: useQueryArgs => useQueryResponseJs('extensions) =
  "useQuery";

/**
 * A function for converting the response to useQuery from the JavaScript
 * representation to a typed Reason record.
 */
let urqlResponseToReason =
    (
      parse: Js.Json.t => 'response,
      result: UrqlTypes.jsResponse(Js.Json.t, 'extensions),
    )
    : UrqlTypes.hookResponse('response, 'extensions) => {
  let data =
    result->UrqlTypes.jsDataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->UrqlTypes.jsErrorGet
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->UrqlTypes.fetchingGet;
  let extensions = result->UrqlTypes.extensionsGet->Js.Nullable.toOption;

  // TODO: figure out how to add stale here.
  let response =
    switch (fetching, data, error) {
    | (true, _, _) => UrqlTypes.Fetching
    | (false, _, Some(error)) => Error(error)
    | (false, Some(data), _) => Data(data)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response, extensions};
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
 * The query will only execute when pause becomes false.
 */
let useQuery =
    (~request, ~requestPolicy=?, ~pause=?, ~pollInterval=?, ~context=?, ()) => {
  let args =
    useQueryArgs(
      ~query=request##query,
      ~variables=request##variables,
      ~requestPolicy=?
        requestPolicy->Belt.Option.map(UrqlTypes.requestPolicyToJs),
      ~pause?,
      ~pollInterval?,
      ~context=?UrqlClient.partialOpCtxToPartialOpCtxJs(context),
      (),
    );

  let (responseJs, executeQueryJs) = useQueryJs(args);

  let response =
    React.useMemo2(
      () => responseJs |> urqlResponseToReason(request##parse),
      (responseJs, request##parse),
    );

  let executeQuery =
    React.useMemo1(
      ((), ~context=?, ()) => {
        let ctxJs = UrqlClient.partialOpCtxToPartialOpCtxJs(context);
        executeQueryJs(ctxJs);
      },
      [|executeQueryJs|],
    );

  (response, executeQuery);
};
