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
let make =
    (
      ~request,
      ~requestPolicy=?,
      ~pause=?,
      ~pollInterval=?,
      ~context=?,
      ~children,
    ) => {
  let (state, executeQuery) =
    UrqlUseQuery.useQuery(
      ~request,
      ~requestPolicy?,
      ~pause?,
      ~pollInterval?,
      ~context?,
      (),
    );

  let result = {
    fetching: state.fetching,
    data: state.data,
    error: state.error,
    response: state.response,
    extensions: state.extensions,
    executeQuery,
  };

  children(result);
};
