let useMutation:
  UrqlTypes.graphqlDefinition(
    'parsedResponse,
    (
      UrqlTypes.hookResponse('parsedResponse),
      unit => Js.Promise.t(UrqlClient.ClientTypes.operationResult),
    ),
    'functionThatAcceptsVarArgsAndReturnsHookResponse,
  ) =>
  'functionThatAcceptsVarArgsAndReturnsHookResponse;

let useDynamicMutation:
  UrqlTypes.graphqlDefinition(
    'parsedResponse,
    Js.Promise.t(UrqlClient.ClientTypes.operationResult),
    'executeMutationFunction,
  ) =>
  (UrqlTypes.hookResponse('parsedResponse), 'executeMutationFunction);
