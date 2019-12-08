let useMutation:
  (~request: UrqlTypes.request('response)) =>
  (
    UrqlTypes.hookResponse('response),
    unit => Js.Promise.t(UrqlClient.ClientTypes.operationResult),
  );

let useDynamicMutation:
  UrqlTypes.graphqlDefinition(
    'parseResult,
    Js.Promise.t(UrqlClient.ClientTypes.operationResult),
    'executeMutation,
  ) =>
  (UrqlTypes.hookResponse('parseResult), 'executeMutation);
