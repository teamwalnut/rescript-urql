type executeMutation =
  (~context: UrqlClientTypes.partialOperationContext=?, unit) =>
  Js.Promise.t(UrqlClientTypes.operationResult);

type useMutationResponse('response, 'extensions) = (
  UrqlTypes.hookResponse('response, 'extensions),
  executeMutation,
);

let useMutation:
  (~request: UrqlTypes.request('response)) =>
  useMutationResponse('response, 'extensions);

let useDynamicMutation:
  UrqlTypes.graphqlDefinition(
    'parse,
    Js.Promise.t(UrqlClientTypes.operationResult),
    'executeMutation,
  ) =>
  (
    UrqlTypes.hookResponse('parse, 'extensions),
    (~context: option(UrqlClientTypes.partialOperationContext)=?) =>
    'executeMutation,
  );
