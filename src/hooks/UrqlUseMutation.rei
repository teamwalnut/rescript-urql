type executeMutation =
  (
    ~fetchOptions: Fetch.requestInit=?,
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~url: string=?,
    ~meta: UrqlTypes.operationDebugMeta=?,
    ~pollInterval: int=?,
    unit
  ) =>
  Js.Promise.t(UrqlTypes.operationResult);

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
    Js.Promise.t(UrqlTypes.operationResult),
    'executeMutation,
  ) =>
  (
    UrqlTypes.hookResponse('parse, 'extensions),
    (
      ~fetchOptions: Fetch.requestInit=?,
      ~requestPolicy: UrqlTypes.requestPolicy=?,
      ~url: string=?,
      ~meta: UrqlTypes.operationDebugMeta=?,
      ~pollInterval: int=?
    ) =>
    'executeMutation,
  );
