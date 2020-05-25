type queryRenderProps('response, 'extensions) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.t),
  executeQuery:
    (~context: UrqlClientTypes.partialOperationContext=?, unit) => unit,
  response: UrqlTypes.response('response),
  extensions: option('extensions),
};

[@react.component]
let make:
  (
    ~request: UrqlTypes.request('response),
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~pause: bool=?,
    ~pollInterval: int=?,
    ~context: UrqlClientTypes.partialOperationContext=?,
    ~children: queryRenderProps('response, 'extensions) => React.element
  ) =>
  React.element;
