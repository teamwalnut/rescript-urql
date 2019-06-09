type handler('a, 'b) =
  (~prevSubscriptions: option('a), ~subscription: 'b) => 'a;

type useSubscriptionResponse('a) = {
  fetching: bool,
  data: option('a),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('a),
};

let useSubscription:
  (
    ~query: string,
    ~variables: Js.Json.t=?,
    ~handler: handler('a, 'b)=?,
    unit
  ) =>
  useSubscriptionResponse('a);