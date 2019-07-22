open UrqlTypes;

[@bs.deriving abstract]
type subscriptionRenderPropsJs('ret) = {
  fetching: bool,
  data: Js.Nullable.t('ret),
  [@bs.optional]
  error: UrqlCombinedError.t,
};

type subscriptionRenderProps('ret) = {
  fetching: bool,
  data: option('ret),
  error: option(UrqlCombinedError.combinedError),
  response: UrqlTypes.response('ret),
};

module SubscriptionJs = {
  [@bs.module "urql"] [@react.component]
  external make:
    (
      ~query: string,
      ~variables: Js.Json.t,
      ~handler: (option('acc), Js.Json.t) => 'acc=?,
      ~children: subscriptionRenderPropsJs('ret) => React.element
    ) =>
    React.element =
    "Subscription";
};

let urqlDataToRecord = (parse, result) => {
  let data = result->dataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->errorGet
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->fetchingGet;

  let response =
    switch (fetching, data, error) {
    | (true, None, _) => Fetching
    | (true, Some(data), _) => Data(data)
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

module Subscription = {
  [@react.component]
  let make =
      (
        ~request: request('response),
        ~children: subscriptionRenderProps('response) => React.element,
      ) => {
    let query = request##query;
    let variables = request##variables;
    let parse = request##parse;

    <SubscriptionJs query variables>
      {result => result |> urqlDataToRecord(parse) |> children}
    </SubscriptionJs>;
  };
};

module SubscriptionWithHandler = {
  [@react.component]
  let make =
      (
        ~request: request('response),
        ~handler: (option('acc), 'response) => 'acc,
        ~children: subscriptionRenderProps('acc) => React.element,
      ) => {
    let query = request##query;
    let variables = request##variables;
    let parse = request##parse;

    let applyParsedResponse = (acc, data) => handler(acc, parse(data));

    <SubscriptionJs query variables handler=applyParsedResponse>
      {result => result |> urqlDataToRecord(x => x) |> children}
    </SubscriptionJs>;
  };
};
