type executeMutation =
  (
    ~additionalTypenames: array(string)=?,
    ~fetchOptions: Fetch.requestInit=?,
    ~fetch: (string, Fetch.requestInit) => Js.Promise.t(Fetch.response)=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~url: string=?,
    ~pollInterval: int=?,
    ~meta: Types.operationDebugMeta=?,
    ~suspense: bool=?,
    ~preferGetMethod: bool=?,
    unit
  ) =>
  Js.Promise.t(Types.operationResult(Js.Json.t));

type useMutationResponse('response) = (
  Types.hookResponse('response),
  executeMutation,
);

let useMutation:
  (~request: Types.request('response)) => useMutationResponse('response);

let useDynamicMutation:
  Types.graphqlDefinition(
    'parse,
    Js.Promise.t(Types.operationResult(Js.Json.t)),
    'executeMutation,
  ) =>
  (
    Types.hookResponse('parse),
    (
      ~additionalTypenames: array(string)=?,
      ~fetchOptions: Fetch.requestInit=?,
      ~fetch: (string, Fetch.requestInit) => Js.Promise.t(Fetch.response)=?,
      ~requestPolicy: Types.requestPolicy=?,
      ~url: string=?,
      ~pollInterval: int=?,
      ~meta: Types.operationDebugMeta=?,
      ~suspense: bool=?,
      ~preferGetMethod: bool=?
    ) =>
    'executeMutation,
  );
