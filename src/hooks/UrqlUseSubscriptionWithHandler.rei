type useSubscriptionWithHandlerResponse('acc) = {
  fetching: bool,
  data: option('acc),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('acc),
};

let useSubscriptionWithHandler:
  (
    ~request: UrqlTypes.request('response),
    ~handler: UrqlTypes.parsedHandler('acc, 'response)
  ) =>
  useSubscriptionWithHandlerResponse('acc);