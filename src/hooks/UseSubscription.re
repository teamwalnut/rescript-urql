/**
 * The handler type used to type the optional accumulator function
 * returned by useSubscription. handler is a GADT used to support
 * proper type inference for useSubscription.
 */;
type handler('acc, 'response, 'ret) =
  | Handler((option('acc), 'response) => 'acc)
    : handler('acc, 'response, 'acc)
  | NoHandler: handler('response, 'response, 'response);

type useSubscriptionArgs = {
  query: string,
  variables: Js.Json.t,
  pause: option(bool),
  context: Types.partialOperationContext,
};

type executeSubscriptionJs = Types.partialOperationContext => unit;

[@bs.module "urql"]
external useSubscriptionJs:
  (useSubscriptionArgs, option((option('acc), Js.Json.t) => 'acc)) =>
  (Types.jsHookResponse('ret, 'extensions), executeSubscriptionJs) =
  "useSubscription";

type executeSubscription =
  (
    ~fetchOptions: Fetch.requestInit=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~url: string=?,
    ~meta: Types.operationDebugMeta=?,
    ~pollInterval: int=?,
    unit
  ) =>
  unit;

type useSubscriptionResponse('response, 'extensions) = (
  Types.hookResponse('response, 'extensions),
  executeSubscription,
);

let subscriptionResponseToReason =
    (result: Types.jsHookResponse('ret, 'extensionns)) => {
  let data = Types.(result.data);
  let error =
    result.error->Belt.Option.map(CombinedError.combinedErrorToRecord);
  let fetching = result.fetching;
  let extensions = result.extensions;

  let response =
    switch (fetching, data, error) {
    | (true, None, _) => Types.Fetching
    | (false, _, Some(error)) => Error(error)
    | (true, Some(data), _) => Data(data)
    | (false, Some(data), _) => Data(data)
    | (false, None, None) => NotFound
    };

  Types.{fetching, data, error, response, extensions};
};

let useSubscription =
    (
      type acc,
      type response,
      type ret,
      ~request: Types.request(response),
      ~handler: handler(acc, response, ret),
      ~pause=?,
      ~fetchOptions=?,
      ~requestPolicy=?,
      ~url=?,
      ~meta=?,
      ~pollInterval=?,
      (),
    ) => {
  let query = request##query;
  let variables = request##variables;
  let parse = request##parse;
  let context =
    React.useMemo5(
      () => {
        Types.partialOperationContext(
          ~fetchOptions?,
          ~url?,
          ~meta?,
          ~pollInterval?,
          ~requestPolicy=?
            Belt.Option.map(requestPolicy, Types.requestPolicyToJs),
          (),
        )
      },
      (fetchOptions, url, meta, pollInterval, requestPolicy),
    );

  let args = {query, variables, pause, context};

  let h = (acc, subscriptionResult) =>
    switch (handler) {
    | Handler(handlerFn) => handlerFn(acc, parse(subscriptionResult))
    | NoHandler => parse(subscriptionResult)
    };

  let (responseJs, executeSubscriptionJs) =
    useSubscriptionJs(args, Some(h));

  let response = subscriptionResponseToReason(responseJs);
  let executeSubscription =
    React.useMemo1(
      (
        (),
        ~fetchOptions=?,
        ~requestPolicy=?,
        ~url=?,
        ~meta=?,
        ~pollInterval=?,
        (),
      ) => {
        let ctx =
          Types.partialOperationContext(
            ~fetchOptions?,
            ~requestPolicy=?
              Belt.Option.map(requestPolicy, Types.requestPolicyToJs),
            ~url?,
            ~meta?,
            ~pollInterval?,
            (),
          );
        executeSubscriptionJs(ctx);
      },
      [|executeSubscriptionJs|],
    );

  (response, executeSubscription);
};
