type handler('acc, 'response, 'ret) =
  | Handler((option('acc), 'response) => 'acc)
    : handler('acc, 'response, 'acc)
  | NoHandler: handler('response, 'response, 'response);

type executeSubscription =
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
  unit;

type useSubscriptionResponse('response) = (
  Types.hookResponse('response),
  executeSubscription,
);

let useSubscription:
  (
    ~request: Types.request('response),
    ~handler: handler('acc, 'response, 'ret),
    ~pause: bool=?,
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
  useSubscriptionResponse('ret);
