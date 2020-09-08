type t = {
  url: string,
  suspense: bool,
  preferGetMethod: bool,
  requestPolicy: string,
  maskTypename: bool,
};

type fetchOptions('a) =
  | FetchOpts(Fetch.requestInit): fetchOptions(Fetch.requestInit)
  | FetchFn(unit => Fetch.requestInit)
    : fetchOptions(unit => Fetch.requestInit);

type fetchImpl('a) =
  | FetchWithUrl(string => Js.Promise.t(Fetch.response))
    : fetchImpl(string => Js.Promise.t(Fetch.response))
  | FetchWithUrlOptions(
      (string, Fetch.requestInit) => Js.Promise.t(Fetch.response),
    )
    : fetchImpl((string, Fetch.requestInit) => Js.Promise.t(Fetch.response))
  | FetchWithRequest(Fetch.request => Js.Promise.t(Fetch.response))
    : fetchImpl(Fetch.request => Js.Promise.t(Fetch.response))
  | FetchWithRequestOptions(
      (Fetch.request, Fetch.requestInit) => Js.Promise.t(Fetch.response),
    )
    : fetchImpl(
        (Fetch.request, Fetch.requestInit) => Js.Promise.t(Fetch.response),
      );

module Exchanges: {
  type client = t;

  type exchangeIO =
    Wonka.Types.sourceT(Types.operation) =>
    Wonka.Types.sourceT(Types.operationResult);

  type exchangeInput = {
    forward: exchangeIO,
    client,
  };

  type t =
    exchangeInput =>
    (. Wonka.Types.sourceT(Types.operation)) =>
    Wonka.Types.sourceT(Types.operationResult);

  [@bs.module "urql"] external cacheExchange: t = "cacheExchange";
  [@bs.module "urql"] external debugExchange: t = "debugExchange";
  [@bs.module "urql"] external dedupExchange: t = "dedupExchange";
  [@bs.module "urql"]
  external fallbackExchangeIO: exchangeIO = "fallbackExchangeIO";
  [@bs.module "urql"] external fetchExchange: t = "fetchExchange";
  [@bs.module "urql"]
  external composeExchanges: array(t) => t = "composeExchanges";
  [@bs.module "urql"]
  external defaultExchanges: array(t) = "defaultExchanges";

  /* Specific types for the subscriptionExchange. */
  type observerLike('value) = {
    next: 'value => unit,
    error: Js.Exn.t => unit,
    complete: unit => unit,
  };

  type unsubscribe = {unsubscribe: unit => unit};

  type observableLike('value) = {
    subscribe: observerLike('value) => unsubscribe,
  };

  type subscriptionOperation = {
    query: string,
    variables: option(Js.Json.t),
    key: string,
    context: Types.operationContext,
  };

  type subscriptionForwarder =
    subscriptionOperation => observableLike(Types.executionResult);

  let subscriptionExchange:
    (
      ~forwardSubscription: subscriptionForwarder,
      ~enableAllOperations: bool=?,
      unit
    ) =>
    t;

  /* Specific types for the ssrExchange. */
  type serializedError = {
    networkError: option(string),
    graphQLErrors: array(string),
  };

  type serializedResult = {
    data: option(Js.Json.t),
    error: option(serializedError),
  };

  type ssrExchangeParams = {
    isClient: option(bool),
    initialState: option(Js.Dict.t(serializedResult)),
  };

  [@bs.send]
  external restoreData: (~exchange: t, ~restore: Js.Json.t) => Js.Json.t =
    "restoreData";
  [@bs.send] external extractData: (~exchange: t) => Js.Json.t = "extractData";
  [@bs.module "urql"]
  external ssrExchange: (~ssrExchangeParams: ssrExchangeParams=?, unit) => t =
    "ssrExchange";
};

let make:
  (
    ~url: string,
    ~fetchOptions: fetchOptions('a)=?,
    ~fetch: fetchImpl('a)=?,
    ~exchanges: array(Exchanges.t)=?,
    ~suspense: bool=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~preferGetMethod: bool=?,
    ~maskTypename: bool=?,
    unit
  ) =>
  t;

type response('response) =
  | Data('response)
  | Error(CombinedError.t)
  | Empty;

type clientResponse('response) = {
  data: option('response),
  error: option(CombinedError.t),
  extensions: option(Js.Dict.t(string)),
  response: response('response),
  stale: option(bool),
};

let urqlClientResponseToReason:
  (~response: Types.operationResult, ~parse: Js.Json.t => 'response) =>
  clientResponse('response);

let executeQuery:
  (
    ~client: t,
    ~request: Types.request('response),
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
  Wonka.Types.sourceT(clientResponse('response));

let executeMutation:
  (
    ~client: t,
    ~request: Types.request('response),
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
  Wonka.Types.sourceT(clientResponse('response));

let executeSubscription:
  (
    ~client: t,
    ~request: Types.request('response),
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
  Wonka.Types.sourceT(clientResponse('response));

let query:
  (
    ~client: t,
    ~request: Types.request('response),
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
  Js.Promise.t(clientResponse('response));

let mutation:
  (
    ~client: t,
    ~request: Types.request('response),
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
  Js.Promise.t(clientResponse('response));

let subscription:
  (
    ~client: t,
    ~request: Types.request('response),
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
  Wonka.Types.sourceT(clientResponse('response));

let readQuery:
  (
    ~client: t,
    ~request: Types.request('response),
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
  option(clientResponse('response));
