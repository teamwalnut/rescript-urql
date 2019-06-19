[@bs.deriving abstract]
type useSubscriptionArgs('acc) = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
};

[@bs.deriving abstract]
type useSubscriptionResponseJs = {
  fetching: bool,
  [@bs.optional]
  data: Js.Json.t,
  [@bs.optional]
  error: UrqlCombinedError.t,
};

type useSubscriptionResponse('response) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('response),
};

[@bs.module "urql"]
external useSubscriptionJs:
  useSubscriptionArgs('acc) => array(useSubscriptionResponseJs) =
  "useSubscription";

let useSubscriptionResponseToRecord =
    (parse: Js.Json.t => 'response, result: useSubscriptionResponseJs) => {
  let data = result->dataGet->Belt.Option.map(parse);
  let error = result->errorGet;
  let fetching = result->fetchingGet;

  let response =
    switch (fetching, data, error) {
    | (true, _, _) => UrqlTypes.Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

let useSubscription = request => {
  let args =
    useSubscriptionArgs(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );
  let state = useSubscriptionJs(args)[0];

  let useSubscriptionResponse =
    state |> useSubscriptionResponseToRecord(request##parse);
  useSubscriptionResponse;
};