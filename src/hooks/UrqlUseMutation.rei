let useMutation:
  (~request: UrqlTypes.request('response)) =>
  (
    UrqlTypes.hookResponse('response, 'extensions),
    (~context: UrqlClientTypes.partialOperationContext=?, unit) =>
    Js.Promise.t(UrqlClientTypes.operationResult),
  );

let useDynamicMutation:
  UrqlTypes.graphqlDefinition(
    'parseResult,
    Js.Promise.t(UrqlClientTypes.operationResult),
    'executeMutation,
  ) =>
  (
    UrqlTypes.hookResponse('parseResult, 'extensions),
    (~context: UrqlClientTypes.partialOperationContext=?, unit) =>
    'executeMutation,
  );
