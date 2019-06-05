type handler('a, 'b) =
  (~prevSubscriptions: option('a), ~subscription: 'b) => 'a;
type useSubscriptionResponseJs('a) = {
  .
  "data": Js.Nullable.t('a),
  "error": Js.Nullable.t(UrqlCombinedError.t),
  "fetching": bool,
};
type useSubscriptionResponse('a) = {
  fetching: bool,
  data: option('a),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('a),
};
external useSubscriptionJs:
  (
    ~query: string,
    ~variables: option(Js.Json.t)=?,
    ~handler: option(handler('a, 'b))=?
  ) =>
  useSubscriptionResponseJs('a) =
  "useSubscription";
let useSubscriptionResponseToRecord:
  useSubscriptionResponseJs('a) => useSubscriptionResponse('a);
let useSubscription:
  (~query: string, ~variables: Js.Json.t=?, ~handler: handler('a, 'b)=?) =>
  useSubscriptionResponse('a);