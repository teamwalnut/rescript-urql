type handler('a, 'b, 'c) =
  | Handler((option('a), 'b) => 'a): handler('a, 'b, 'a)
  | NoHandler: handler(_, 'b, 'b);

type useSubscriptionResponse('acc) = {
  fetching: bool,
  data: option('acc),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('acc),
};

let useSubscription:
  (
    ~handler: handler('a, 'b, 'c),
    UrqlTypes.request('b)
  ) =>
  useSubscriptionResponse('c);
