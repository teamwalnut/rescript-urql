type rec handler<'acc, 'response, 'ret> =
  | Handler((option<'acc>, 'response) => 'acc): handler<'acc, 'response, 'acc>
  | NoHandler: handler<'response, 'response, 'response>

type executeSubscription = (
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

type useSubscriptionResponse<'data> = (Types.Hooks.hookResponse<'data>, executeSubscription)

let useSubscription: (
  ~subscription: module(Types.Operation with type t = 'data and type t_variables = 'variables),
  ~handler: handler<'acc, 'data, 'ret>,
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
) => useSubscriptionResponse<'ret>
