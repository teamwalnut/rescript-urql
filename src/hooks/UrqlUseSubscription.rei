open UrqlTypes;

type handler('acc, 'resp, 'ret) =
  | Handler((option('acc), 'resp) => 'acc): handler('acc, 'resp, 'acc)
  | NoHandler: handler(_, 'resp, 'resp);

let useSubscription:
  (
    ~handler: handler('acc, 'resp, 'ret),
    ~request: UrqlTypes.request('resp)
  ) =>
  hookResponse('ret);
