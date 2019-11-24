type executeQuery =
  option(UrqlClient.ClientTypes.partialOperationContext) => unit;

type useQueryResponse('response, 'extensions) = (
  UrqlTypes.hookResponse('response, 'extensions),
  executeQuery,
);

let useQuery:
  (
    ~request: UrqlTypes.request('response),
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~pause: bool=?,
    ~context: UrqlClient.ClientTypes.partialOperationContext=?,
    unit
  ) =>
  useQueryResponse('response, 'extensions);
