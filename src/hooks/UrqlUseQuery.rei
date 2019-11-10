type executeQuery =
  option(UrqlClient.ClientTypes.partialOperationContext) => unit;

type useQueryResponse('response) = (
  UrqlTypes.hookResponse('response),
  executeQuery,
);

let useQuery:
  (
    ~request: UrqlTypes.request('response),
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~pause: bool=?,
    unit
  ) =>
  useQueryResponse('response);
