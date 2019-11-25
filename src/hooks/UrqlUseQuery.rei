type executeQuery =
  option(UrqlClient.ClientTypes.partialOperationContext) => unit;

type useQueryResponse('response) = (
  UrqlTypes.hookResponse('response),
  executeQuery,
);

let useQuery:
  (
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~pause: bool=?,
    UrqlTypes.graphqlDefinition(
      'parsedResponse,
      useQueryResponse('parsedResponse),
      'functionThatAcceptsVarArgsAndReturnsHookResponse,
    )
  ) =>
  'functionThatAcceptsVarArgsAndReturnsHookResponse;
