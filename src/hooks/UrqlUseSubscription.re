[@bs.module "urql"]
type handler('a, 'b) =
  (~prevSubscriptions: option('a), ~subscription: 'b) => 'a;

type useSubscriptionResponseJs('a) = {
  .
  "fetching": bool,
  "data": Js.Nullable.t('a),
  "error": Js.Nullable.t(UrqlCombinedError.t),
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

let useSubscriptionResponseToRecord = (result: useSubscriptionResponseJs('a)) => {
  let data = result##data |> Js.Nullable.toOption;
  let error = result##error |> Js.Nullable.toOption;

  let response: UrqlTypes.response('a) =
    switch (result##fetching, data, error) {
    | (true, _, _) => Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching: result##fetching, data, error, response};
};

let useSubscription =
    (
      ~query: string,
      ~variables: option(Js.Json.t)=?,
      ~handler: option(handler('a, 'b))=?,
    ) => {
  let useSubscriptionResponseJs =
    switch (variables, handler) {
    | (Some(v), Some(h)) =>
      useSubscriptionJs(~query, ~variables=Some(v), ~handler=Some(h))
    | (Some(v), None) =>
      useSubscriptionJs(~query, ~variables=Some(v), ~handler=None)
    | (None, Some(h)) =>
      useSubscriptionJs(~query, ~variables=None, ~handler=Some(h))
    | (None, None) =>
      useSubscriptionJs(~query, ~variables=None, ~handler=None)
    };

  let useSubscriptionResponse =
    useSubscriptionResponseJs |> useSubscriptionResponseToRecord;
  useSubscriptionResponse;
};