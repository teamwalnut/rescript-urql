type executeMutation('variables, 'data) =
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
    'variables
  ) =>
  Js.Promise.t(Types.operationResult('data));

type useMutationResponse('variables, 'data) = (
  Types.Hooks.hookResponse('data),
  executeMutation('variables, 'data),
);

let useMutation:
  (
    ~mutation: (module Types.Operation with
                  type t = 'data and type t_variables = 'variables)
  ) =>
  useMutationResponse('variables, 'data);
