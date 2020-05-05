type executeSubscription =
  (~context: UrqlClientTypes.partialOperationContext=?, unit) => unit;

type subscriptionRenderProps('response, 'extensions) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.t),
  executeSubscription,
  response: UrqlTypes.response('response),
  extensions: option('extensions),
};

[@react.component]
let make:
  (
    ~request: UrqlTypes.request('response),
    ~pause: bool=?,
    ~context: UrqlClientTypes.partialOperationContext=?,
    ~children: subscriptionRenderProps('response, 'extensions) =>
               React.element
  ) =>
  React.element;
