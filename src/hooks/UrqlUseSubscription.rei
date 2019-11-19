type handler('acc, 'resp, 'ret) =
  | Handler((option('acc), 'resp) => 'acc): handler('acc, 'resp, 'acc)
  | NoHandler: handler(_, 'resp, 'resp);

let useSubscription:
  (
    ~request: UrqlTypes.request('resp),
    ~handler: handler('acc, 'resp, 'ret)
  ) =>
  UrqlTypes.hookResponse('ret);

module type SubscriptionConfig = {
  type t;
  let query: string;
  let parse: Js.Json.t => t;
};

module type MakeSubscriptionType = {
  type resp;

  let useSubscription:
    (~variables: Js.Json.t=?, ~handler: handler('acc, resp, 'ret)) =>
    UrqlTypes.hookResponse('ret);
};

module MakeSubscription:
  (Subscription: SubscriptionConfig) =>
   MakeSubscriptionType with type resp = Subscription.t;
