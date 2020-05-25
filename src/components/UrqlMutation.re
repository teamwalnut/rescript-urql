type mutationRenderProps('response, 'extensions) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.t),
  executeMutation:
    (~context: UrqlClientTypes.partialOperationContext=?, unit) =>
    Js.Promise.t(UrqlClientTypes.operationResult),
  response: UrqlTypes.response('response),
  extensions: option('extensions),
};

[@react.component]
let make = (~request, ~children) => {
  let (state, executeMutation) = UrqlUseMutation.useMutation(~request);

  let result = {
    fetching: state.fetching,
    data: state.data,
    error: state.error,
    response: state.response,
    extensions: state.extensions,
    executeMutation,
  };

  children(result);
};
