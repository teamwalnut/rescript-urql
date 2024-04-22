type executeQuery = (
  ~additionalTypenames: array<string>=?,
  ~fetchOptions: Fetch.Request.init=?,
  ~fetch: (string, Fetch.Request.init) => Js.Promise.t<Fetch.Response.t>=?,
  ~requestPolicy: Types.requestPolicy=?,
  ~url: string=?,
  ~meta: Types.operationDebugMeta=?,
  ~suspense: bool=?,
  ~preferGetMethod: bool=?,
  unit,
) => unit

type useQueryResponse<'data> = (Types.Hooks.hookResponse<'data>, executeQuery)

let useQuery: (
  ~query: module(Types.Operation with
    type t_variables = 'variables
    and type Raw.t_variables = 'variablesJs
    and type t = 'data
  ),
  ~pause: bool=?,
  ~additionalTypenames: array<string>=?,
  ~fetchOptions: Fetch.Request.init=?,
  ~fetch: (string, Fetch.Request.init) => Js.Promise.t<Fetch.Response.t>=?,
  ~requestPolicy: Types.requestPolicy=?,
  ~url: string=?,
  ~meta: Types.operationDebugMeta=?,
  ~suspense: bool=?,
  ~preferGetMethod: bool=?,
  'variables,
) => useQueryResponse<'data>
