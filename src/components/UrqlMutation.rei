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
let make:
  (
    ~request: UrqlTypes.request('response),
    ~children: mutationRenderProps('response, 'extensions) => React.element
  ) =>
  React.element;
