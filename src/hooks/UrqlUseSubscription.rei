type handler('acc, 'response, 'ret) =
  | Handler((option('acc), 'response) => 'acc)
    : handler('acc, 'response, 'acc)
  | NoHandler: handler('response, 'response, 'response);

type executeSubscription =
  (
    ~fetchOptions: Fetch.requestInit=?,
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~url: string=?,
    ~meta: UrqlTypes.operationDebugMeta=?,
    ~pollInterval: int=?,
    unit
  ) =>
  unit;

type useSubscriptionResponse('response, 'extensions) = (
  UrqlTypes.hookResponse('response, 'extensions),
  executeSubscription,
);

let useSubscription:
  (
    ~request: UrqlTypes.request('response),
    ~handler: handler('acc, 'response, 'ret),
    ~pause: bool=?,
    ~fetchOptions: Fetch.requestInit=?,
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~url: string=?,
    ~meta: UrqlTypes.operationDebugMeta=?,
    ~pollInterval: int=?,
    unit
  ) =>
  useSubscriptionResponse('ret, 'extensions);
