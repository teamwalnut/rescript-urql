[@bs.deriving abstract]
type subscriptionRenderPropsJs('ret, 'extensions) = {
  fetching: bool,
  data: Js.Nullable.t('ret),
  error: Js.Nullable.t(UrqlCombinedError.combinedErrorJs),
  extensions: Js.Nullable.t('extensions),
};

type subscriptionRenderProps('ret, 'extensions) = {
  fetching: bool,
  data: option('ret),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('ret),
  extensions: option('extensions),
};

module SubscriptionJs = {
  [@bs.module "urql"] [@react.component]
  external make:
    (
      ~query: string,
      ~variables: Js.Json.t,
      ~handler: (option('acc), Js.Json.t) => 'acc=?,
      ~context: UrqlClient.ClientTypes.partialOperationContext=?,
      ~children: subscriptionRenderPropsJs('ret, 'extensions) => React.element
    ) =>
    React.element =
    "Subscription";
};

let urqlDataToRecord = (parse, result) => {
  let data = result->dataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->errorGet
    ->Js.Nullable.toOption
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->fetchingGet;
  let extensions = result->extensionsGet->Js.Nullable.toOption;

  let response =
    switch (fetching, data, error) {
    | (true, None, _) => UrqlTypes.Fetching
    | (false, _, Some(error)) => Error(error)
    | (true, Some(data), _) => Data(data)
    | (false, Some(data), _) => Data(data)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response, extensions};
};

module Subscription = {
  [@react.component]
  let make =
      (
        ~request: UrqlTypes.request('response),
        ~context: option(UrqlClient.ClientTypes.partialOperationContext)=?,
        ~children:
           subscriptionRenderProps('response, 'extensions) => React.element,
      ) => {
    let query = request##query;
    let variables = request##variables;
    let parse = request##parse;

    <SubscriptionJs query variables ?context>
      {result => result |> urqlDataToRecord(parse) |> children}
    </SubscriptionJs>;
  };
};

module SubscriptionWithHandler = {
  [@react.component]
  let make =
      (
        ~request: UrqlTypes.request('response),
        ~handler: (option('acc), 'response) => 'acc,
        ~context: option(UrqlClient.ClientTypes.partialOperationContext)=?,
        ~children:
           subscriptionRenderProps('acc, 'extensions) => React.element,
      ) => {
    let query = request##query;
    let variables = request##variables;
    let parse = request##parse;

    let applyParsedResponse = (acc, data) => handler(acc, parse(data));

    <SubscriptionJs query variables handler=applyParsedResponse ?context>
      {result => result |> urqlDataToRecord(x => x) |> children}
    </SubscriptionJs>;
  };
};
