type executeMutation =
  (
    ~fetchOptions: Fetch.requestInit=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~url: string=?,
    ~meta: Types.operationDebugMeta=?,
    ~pollInterval: int=?,
    unit
  ) =>
  Js.Promise.t(Types.operationResult);

type useMutationResponse('response, 'extensions) = (
  Types.hookResponse('response, 'extensions),
  executeMutation,
);

let useMutation:
  (~request: Types.request('response)) =>
  useMutationResponse('response, 'extensions);

let useDynamicMutation:
  Types.graphqlDefinition(
    'parse,
    Js.Promise.t(Types.operationResult),
    'executeMutation,
  ) =>
  (
    Types.hookResponse('parse, 'extensions),
    (
      ~fetchOptions: Fetch.requestInit=?,
      ~requestPolicy: Types.requestPolicy=?,
      ~url: string=?,
      ~meta: Types.operationDebugMeta=?,
      ~pollInterval: int=?
    ) =>
    'executeMutation,
  );
