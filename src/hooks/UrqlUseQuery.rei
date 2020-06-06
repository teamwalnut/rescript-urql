type executeQuery =
  (
    ~fetchOptions: Fetch.requestInit=?,
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~url: string=?,
    ~meta: UrqlOperations.operationDebugMeta=?,
    ~pollInterval: int=?,
    unit
  ) =>
  unit;

type useQueryResponse('response, 'extensions) = (
  UrqlTypes.hookResponse('response, 'extensions),
  executeQuery,
);

let useQuery:
  (
    ~request: UrqlTypes.request('response),
    ~pause: bool=?,
    ~fetchOptions: Fetch.requestInit=?,
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~url: string=?,
    ~meta: UrqlOperations.operationDebugMeta=?,
    ~pollInterval: int=?,
    unit
  ) =>
  useQueryResponse('response, 'extensions);
