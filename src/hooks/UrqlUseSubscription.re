/**
 * The handler type used to type the optional accumulator function
 * returned by useSubscription. handler is a GADT used to support
 * proper type inference for useSubscription.
 */
type handler('acc, 'resp, 'ret) =
  | Handler((option('acc), 'resp) => 'acc): handler('acc, 'resp, 'acc)
  | NoHandler: handler('resp, 'resp, 'resp);

/* Arguments passed to useSubscription on the JavaScript side. */
[@bs.deriving abstract]
type useSubscriptionArgs = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  context: UrqlClient.ClientTypes.partialOperationContext,
};

type executeSubscriptionJs;

[@bs.module "urql"]
external useSubscriptionJs:
  (useSubscriptionArgs, option((option('acc), Js.Json.t) => 'acc)) =>
  (UrqlTypes.jsResponse('ret, 'extensions), executeSubscriptionJs) =
  "useSubscription";

/**
 * A function for converting the response to useQuery from the JavaScript
 * representation to a typed Reason record.
 */
let jsSubscriptionResponseToRecord =
    (result): UrqlTypes.hookResponse('response, 'extensions) => {
  let data = result->UrqlTypes.jsDataGet->Js.Nullable.toOption;
  let error =
    result
    ->UrqlTypes.jsErrorGet
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->UrqlTypes.fetchingGet;
  let extensions = result->UrqlTypes.extensionsGet->Js.Nullable.toOption;

  let response =
    switch (fetching, data, error) {
    | (true, None, _) => UrqlTypes.Fetching
    | (false, _, Some(error)) => Error(error)
    | (true, Some(data), _) => Data(data)
    | (false, Some(data), _) => Data(data)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response, extensions};
};

/**
 * The useSubscription hook.
 *
 * Accepts the following arguments:
 *
 * request – a Js.t containing the query and variables corresponding
 * to the GraphQL subscription, and a parse function for decoding the JSON response.
 *
 * handler – an optional function to accumulate subscription responses.
 */
let useSubscription =
    (
      type acc,
      type resp,
      type ret,
      ~request: UrqlTypes.request(resp),
      ~handler: handler(acc, resp, ret),
      ~context=?,
      (),
    )
    : UrqlTypes.hookResponse(ret, 'extensions) => {
  let parse = request##parse;

  let args =
    useSubscriptionArgs(
      ~query=request##query,
      ~variables=request##variables,
      ~context?,
      (),
    );

  let handler' = (acc, jsData) =>
    switch (handler) {
    | Handler(handlerFn) => handlerFn(acc, parse(jsData))
    | NoHandler => parse(jsData)
    };

  let (jsResponse, _) = useSubscriptionJs(args, Some(handler'));

  UrqlGuaranteedMemo.useGuaranteedMemo1(
    jsSubscriptionResponseToRecord,
    jsResponse,
  );
};
