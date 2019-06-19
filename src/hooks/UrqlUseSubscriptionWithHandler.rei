type useSubscriptionWithHandlerResponse('acc) = {
  fetching: bool,
  data: option('acc),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('acc),
};

let useSubscriptionWithHandler:
  (UrqlTypes.request('response), UrqlTypes.parsedHandler('acc, 'response)) =>
  useSubscriptionWithHandlerResponse('acc);