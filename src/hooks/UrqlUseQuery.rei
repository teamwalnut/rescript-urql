type executeQuery =
  (~context: UrqlClientTypes.partialOperationContext=?, unit) => unit;

type useQueryResponse('response, 'extensions) = (
  UrqlTypes.hookResponse('response, 'extensions),
  executeQuery,
);

let useQuery:
  (
    ~request: UrqlTypes.request('response),
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~pause: bool=?,
    ~pollInterval: int=?,
    ~context: UrqlClientTypes.partialOperationContext=?,
    unit
  ) =>
  useQueryResponse('response, 'extensions);
