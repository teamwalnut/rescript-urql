[@bs.deriving abstract]
type useSubscriptionWithHandlerArgs = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
};

[@bs.deriving abstract]
type useSubscriptionWithHandlerResponseJs('acc) = {
  fetching: bool,
  [@bs.optional]
  data: 'acc,
  [@bs.optional]
  error: UrqlCombinedError.t,
};

type useSubscriptionWithHandlerResponse('acc) = {
  fetching: bool,
  data: option('acc),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('acc),
};

[@bs.module "urql"]
external useSubscriptionWithHandlerJs:
  (
    useSubscriptionWithHandlerArgs,
    UrqlTypes.parsedHandler('acc, 'response)
  ) =>
  array(useSubscriptionWithHandlerResponseJs('acc)) =
  "useSubscription";

let useSubscriptionResponseToRecordWithHandler =
    (result: useSubscriptionWithHandlerResponseJs('acc)) => {
  let data = result->dataGet;
  let error = result->errorGet;
  let fetching = result->fetchingGet;

  let response =
    switch (fetching, data, error) {
    | (true, None, _) => UrqlTypes.Fetching
    | (true, Some(d), _) => Data(d)
    | (false, Some(d), _) => Data(d)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

let useSubscriptionWithHandler =
    (
      request: UrqlTypes.request('response),
      handler: UrqlTypes.parsedHandler('acc, 'response),
    ) => {
  let parse = request##parse;
  let parsedHandler =
      (~prevSubscriptions: option('acc), ~subscription: Js.Json.t) =>
    handler(~prevSubscriptions, ~subscription=parse(subscription));

  let args =
    useSubscriptionWithHandlerArgs(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );

  let state = useSubscriptionWithHandlerJs(args, parsedHandler)[0];
  let useSubscriptionResponse =
    state |> useSubscriptionResponseToRecordWithHandler;
  useSubscriptionResponse;
};