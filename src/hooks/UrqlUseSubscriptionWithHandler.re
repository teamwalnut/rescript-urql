type handler('a, 'b, 'c) =
  | Handler((option('a), 'b) => 'a): handler('a, 'b, 'a)
  | NoHandler: handler(_, 'b, 'b);

[@bs.deriving abstract]
type useSubscriptionArgs = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
};

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

[@bs.module "urql"]
external useSubscriptionJs:
  (
    useSubscriptionArgs,
    option((option('a), 'b) => 'a)
  ) =>
  array(useSubscriptionResponseJs('c)) =
  "useSubscription";

let useSubscriptionResponseToRecord =
    (parse, result) => {
  let data = result->dataGet->Belt.Option.map(parse);
  let error = result->errorGet;
  let fetching = result->fetchingGet;

  let response =
    switch (fetching, data, error) {
    | (true, None, _) => UrqlTypes.Fetching
    | (true, Some(data), _) => Data(data)
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

let useSubscription =
    (
      type a, type b, type c,
      ~handler: handler(a, b, c),
      request: UrqlTypes.request(b),
    ): useSubscriptionResponse(c) => {
  let parse = request##parse;
  let args =
    useSubscriptionArgs(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );

  let state: useSubscriptionResponse(c) = switch (handler) {
    | NoHandler => 
      useSubscriptionJs(args, None)[0] 
      |> useSubscriptionResponseToRecord(parse)
    | Handler(handler_fn) =>
      useSubscriptionJs(args, Some(handler_fn))[0] 
      |> useSubscriptionResponseToRecord(x => x)
  }

  state
};
