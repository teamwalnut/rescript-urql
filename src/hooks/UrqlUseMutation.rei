let useMutation:
  (~request: UrqlTypes.request('response)) =>
  (
    UrqlTypes.hookResponse('response),
    unit => Js.Promise.t(UrqlClient.ClientTypes.operationResult),
  );

let useDynamicMutation:
  UrqlTypes.graphqlDefinition(
    'parsedResponse,
    Js.Promise.t(UrqlClient.ClientTypes.operationResult),
    'executeMutationFunction,
  ) =>
  (UrqlTypes.hookResponse('parsedResponse), 'executeMutationFunction);
