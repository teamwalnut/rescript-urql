/**
 * The handler type used to type the optional accumulator function
 * returned by useSubscription. handler is a GADT used to support
 * proper type inference for polymorphic useSubscription.
 */
type handler('acc, 'response, 'ret) =
  | Handler((option('acc), 'response) => 'acc)
    : handler('acc, 'response, 'acc)
  | NoHandler: handler('response, 'response, 'response);

type useSubscriptionArgs = {
  query: string,
  variables: Js.Json.t,
  pause: option(bool),
  context: option(UrqlClientTypes.partialOperationContextJs),
};

type executeSubscriptionJs =
  option(UrqlClientTypes.partialOperationContextJs) => unit;

[@bs.module "urql"]
external useSubscriptionJs:
  (useSubscriptionArgs, option((option('acc), Js.Json.t) => 'acc)) =>
  (UrqlTypes.jsHookResponse('ret, 'extensions), executeSubscriptionJs) =
  "useSubscription";

type executeSubscription =
  (~context: UrqlClientTypes.partialOperationContext=?, unit) => unit;

type useSubscriptionResponse('response, 'extensions) = (
  UrqlTypes.hookResponse('response, 'extensions),
  executeSubscription,
);

let subscriptionResponseToReason = result => {
  let data = UrqlTypes.(result.data);
  let error =
    result.error->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result.fetching;
  let extensions = result.extensions;

  let response =
    switch (fetching, data, error) {
    | (true, None, _) => UrqlTypes.Fetching
    | (false, _, Some(error)) => Error(error)
    | (true, Some(data), _) => Data(data)
    | (false, Some(data), _) => Data(data)
    | (false, None, None) => NotFound
    };

  UrqlTypes.{fetching, data, error, response, extensions};
};

let useSubscription =
    (
      type acc,
      type response,
      type ret,
      ~request: UrqlTypes.request(response),
      ~handler: handler(acc, response, ret),
      ~pause=?,
      ~context=?,
      (),
    ) => {
  let parse = request##parse;

  let args = {
    query: request##query,
    variables: request##variables,
    pause,
    context: UrqlClientTypes.decodeOperationRequestPolicy(context),
  };

  let h = (acc, subscriptionResult) =>
    switch (handler) {
    | Handler(handlerFn) => handlerFn(acc, parse(subscriptionResult))
    | NoHandler => parse(subscriptionResult)
    };

  let (responseJs, executeSubscriptionJs) =
    useSubscriptionJs(args, Some(h));

  let response = subscriptionResponseToReason(responseJs);
  let executeSubscription = (~context=?, ()) => {
    let ctx = UrqlClientTypes.decodeOperationRequestPolicy(context);
    executeSubscriptionJs(ctx);
  };

  (response, executeSubscription);
};
