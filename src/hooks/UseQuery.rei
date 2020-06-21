type executeQuery =
  (
    ~fetchOptions: Fetch.requestInit=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~url: string=?,
    ~meta: Types.operationDebugMeta=?,
    ~pollInterval: int=?,
    unit
  ) =>
  unit;

type useQueryResponse('response, 'extensions) = (
  Types.hookResponse('response, 'extensions),
  executeQuery,
);

let useQuery:
  (
    ~request: Types.request('response),
    ~pause: bool=?,
    ~fetchOptions: Fetch.requestInit=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~url: string=?,
    ~meta: Types.operationDebugMeta=?,
    ~pollInterval: int=?,
    unit
  ) =>
  useQueryResponse('response, 'extensions);
