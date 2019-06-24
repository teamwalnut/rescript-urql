type useSubscriptionResponse('response) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('response),
};

let useSubscription:
  UrqlTypes.request('response) => useSubscriptionResponse('response);
