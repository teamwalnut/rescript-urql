type handler('acc, 'resp, 'ret) =
  | Handler((option('acc), 'resp) => 'acc): handler('acc, 'resp, 'acc)
  | NoHandler: handler(_, 'resp, 'resp);

let useSubscription:
  (
    ~request: UrqlTypes.request('resp),
    ~handler: handler('acc, 'resp, 'ret),
    ~context: UrqlClient.ClientTypes.partialOperationContext=?,
    unit
  ) =>
  UrqlTypes.hookResponse('ret, 'extensions);
