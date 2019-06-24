type handler('acc, 'resp, 'ret) =
  | Handler((option('acc), 'resp) => 'acc): handler('acc, 'resp, 'acc)
  | NoHandler: handler(_, 'resp, 'resp);

type useSubscriptionResponse('ret) = {
  fetching: bool,
  data: option('ret),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('ret),
};

let useSubscription:
  (
    ~handler: handler('acc, 'resp, 'ret),
    ~request: UrqlTypes.request('resp)
  ) =>
  useSubscriptionResponse('ret);
