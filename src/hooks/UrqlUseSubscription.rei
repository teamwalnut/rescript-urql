type handler('acc, 'response, 'ret) =
  | Handler((option('acc), 'response) => 'acc)
    : handler('acc, 'response, 'acc)
  | NoHandler: handler('response, 'response, 'response);

type executeSubscription =
  (~context: UrqlClientTypes.partialOperationContext=?, unit) => unit;

type useSubscriptionResponse('response, 'extensions) = (
  UrqlTypes.hookResponse('response, 'extensions),
  executeSubscription,
);

let useSubscription:
  (
    ~request: UrqlTypes.request('response),
    ~handler: handler('acc, 'response, 'ret),
    ~pause: bool=?,
    ~context: UrqlClientTypes.partialOperationContext=?,
    unit
  ) =>
  useSubscriptionResponse('ret, 'extensions);
