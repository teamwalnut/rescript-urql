type handler('a, 'b) =
  (~prevSubscriptions: option('a), ~subscription: 'b) => 'a;

[@bs.deriving abstract]
type useSubscriptionResponseJs('a) = {
  fetching: bool,
  [@bs.optional]
  data: 'a,
  [@bs.optional]
  error: UrqlCombinedError.t,
};

type useSubscriptionResponse('a) = {
  fetching: bool,
  data: option('a),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('a),
};

[@bs.deriving abstract]
type useSubscriptionArgs('a, 'b) = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  handler: handler('a, 'b),
};

[@bs.module "urql"]
external useSubscriptionJs:
  useSubscriptionArgs('a, 'b) => useSubscriptionResponseJs('a) =
  "useSubscription";

let useSubscriptionResponseToRecord = (result: useSubscriptionResponseJs('a)) => {
  let data = result->dataGet;
  let error = result->errorGet;
  let fetching = result->fetchingGet;

  let response: UrqlTypes.response('a) =
    switch (fetching, data, error) {
    | (true, _, _) => Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

let useSubscription =
    (
      ~query: string,
      ~variables: option(Js.Json.t)=?,
      ~handler: option(handler('a, 'b))=?,
      (),
    ) => {
  let args = useSubscriptionArgs(~query, ~variables?, ~handler?, ());
  let state = useSubscriptionJs(args);

  let useSubscriptionResponse = state |> useSubscriptionResponseToRecord;
  useSubscriptionResponse;
};