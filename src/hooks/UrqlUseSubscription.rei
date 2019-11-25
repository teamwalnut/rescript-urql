type handler('acc, 'resp, 'ret) =
  | Handler((option('acc), 'resp) => 'acc): handler('acc, 'resp, 'acc)
  | NoHandler: handler(_, 'resp, 'resp);

let useSubscription:
  (
    ~handler: handler('acc, 'parsedResponse, 'ret),
    UrqlTypes.graphqlDefinition(
      'parsedResponse,
      UrqlTypes.hookResponse('ret),
      'functionThatAcceptsVarArgsAndReturnsHookResponse,
    )
  ) =>
  'functionThatAcceptsVarArgsAndReturnsHookResponse;
