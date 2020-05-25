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
let make = (~request, ~pause=?, ~context=?, ~children) => {
  let (state, executeSubscription) =
    UrqlUseSubscription.useSubscription(
      ~request,
      ~handler=UrqlUseSubscription.NoHandler,
      ~pause?,
      ~context?,
      (),
    );

  let result = {
    fetching: state.fetching,
    data: state.data,
    error: state.error,
    response: state.response,
    extensions: state.extensions,
    executeSubscription,
  };

  children(result);
};
