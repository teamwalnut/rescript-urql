/**
 * The handler type used to type the optional accumulator function
 * returned by useSubscription. handler is a GADT used to support
 * proper type inference for useSubscription.
 */
type handler('acc, 'resp, 'ret) =
  | Handler((option('acc), 'resp) => 'acc): handler('acc, 'resp, 'acc)
  | NoHandler: handler(_, 'resp, 'resp);

/* Arguments passed to useSubscription on the JavaScript side. */
[@bs.deriving abstract]
type useSubscriptionArgs = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  context: UrqlClient.ClientTypes.partialOperationContext,
};

[@bs.module "urql"]
external useSubscriptionJs:
  (useSubscriptionArgs, option((option('acc), Js.Json.t) => 'acc)) =>
  array(UrqlTypes.jsResponse('ret, 'extensions)) =
  "useSubscription";

/**
 * A function for converting the response to useQuery from the JavaScript
 * representation to a typed Reason record.
 */
let useSubscriptionResponseToRecord =
    (parse, result): UrqlTypes.hookResponse('response, 'extensions) => {
  let data =
    result->UrqlTypes.jsDataGet->Js.Nullable.toOption->Belt.Option.map(parse);
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
 */;
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

  React.useMemo3(
    () => {
      let response: UrqlTypes.hookResponse(ret, 'extensions) =
        switch (handler) {
        | Handler(handlerFn) =>
          useSubscriptionJs(
            args,
            Some((acc, data) => handlerFn(acc, parse(data))),
          )[0]
          |> useSubscriptionResponseToRecord(x => x)
        | NoHandler =>
          useSubscriptionJs(args, None)[0]
          |> useSubscriptionResponseToRecord(parse)
        };

      response;
    },
    (handler, args, parse),
  );
};
