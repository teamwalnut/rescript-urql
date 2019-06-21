type useSubscriptionResponse('acc) = {
  fetching: bool,
  data: option('acc),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('acc),
};

let useSubscription:
  (
    ~handler: (option('a), 'b) => 'a=?,
    UrqlTypes.request('b)
  ) =>
  useSubscriptionResponse('c);
