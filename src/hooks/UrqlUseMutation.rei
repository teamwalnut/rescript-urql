let useMutation:
  (~request: UrqlTypes.request('response)) =>
  (
    UrqlTypes.hookResponse('response, 'extensions),
    (~context: UrqlClient.ClientTypes.partialOperationContext=?, unit) =>
    Js.Promise.t(UrqlClient.ClientTypes.operationResult),
  );

let useDynamicMutation:
  UrqlTypes.graphqlDefinition(
    'parseResult,
    Js.Promise.t(UrqlClient.ClientTypes.operationResult),
    'executeMutation,
  ) =>
  (
    UrqlTypes.hookResponse('parseResult, 'extensions),
    (~context: option(UrqlClient.ClientTypes.partialOperationContext)=?) =>
    'executeMutation,
  );
