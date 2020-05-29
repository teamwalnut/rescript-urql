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
  context: option(UrqlClientTypes.PartialOperationContextJs.t),
};

type executeSubscriptionJs =
  option(UrqlClientTypes.PartialOperationContextJs.t) => unit;

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

let subscriptionResponseToReason = (~response, ~hasExecuted) => {
  let data = UrqlTypes.(response.data);
  let error =
    response.error->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = response.fetching;
  let extensions = response.extensions;

  let response =
    switch (fetching, data, error, hasExecuted) {
    | (false, None, None, false) => UrqlTypes.Init
    | (true, None, _, _) => Fetching
    | (_, Some(data), _, _) => Data(data)
    | (false, _, Some(error), _) => Error(error)
    | (false, None, None, true) => Empty
    };

  UrqlTypes.{fetching, data, error, response, extensions};
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
 *
 * pause – an optional boolean flag instructing execution to be paused. The
 * subscription will only be run when pause becomes true.
 *
 * context – a partial operation context to alter the execution conditions of
 * the subscription.
 */;
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
  let query = request##query;
  let variables = request##variables;
  let parse = request##parse;

  let args = {
    query,
    variables,
    pause,
    context: context->UrqlClientTypes.decodePartialOperationContext,
  };

  let h = (acc, subscriptionResult) =>
    switch (handler) {
    | Handler(handlerFn) => handlerFn(acc, parse(subscriptionResult))
    | NoHandler => parse(subscriptionResult)
    };

  let (responseJs, executeSubscriptionJs) =
    useSubscriptionJs(args, Some(h));
  let hasExecuted = React.useRef(false);

  let response =
    React.useMemo2(
      () =>
        subscriptionResponseToReason(
          ~response=responseJs,
          ~hasExecuted=
            hasExecuted->React.Ref.current
            || !pause->Belt.Option.getWithDefault(false),
        ),
      (responseJs, pause),
    );

  let executeSubscription =
    React.useMemo1(
      ((), ~context=?, ()) => {
        if (!hasExecuted->React.Ref.current) {
          hasExecuted->React.Ref.setCurrent(true);
        };

        let ctx = UrqlClientTypes.decodePartialOperationContext(context);
        executeSubscriptionJs(ctx);
      },
      [|executeSubscriptionJs|],
    );

  (response, executeSubscription);
};
