type t = {
  url: string,
  suspense: bool,
  preferGetMethod: bool,
  requestPolicy: string,
  maskTypename: bool,
}

type rec fetchOptions<'a> =
  | FetchOpts(Fetch.requestInit): fetchOptions<Fetch.requestInit>
  | FetchFn(unit => Fetch.requestInit): fetchOptions<unit => Fetch.requestInit>

type rec fetchImpl<'a> =
  | FetchWithUrl(string => Js.Promise.t<Fetch.response>): fetchImpl<
      string => Js.Promise.t<Fetch.response>,
    >
  | FetchWithUrlOptions((string, Fetch.requestInit) => Js.Promise.t<Fetch.response>): fetchImpl<
      (string, Fetch.requestInit) => Js.Promise.t<Fetch.response>,
    >
  | FetchWithRequest(Fetch.request => Js.Promise.t<Fetch.response>): fetchImpl<
      Fetch.request => Js.Promise.t<Fetch.response>,
    >
  | FetchWithRequestOptions(
      (Fetch.request, Fetch.requestInit) => Js.Promise.t<Fetch.response>,
    ): fetchImpl<(Fetch.request, Fetch.requestInit) => Js.Promise.t<Fetch.response>>

module Exchanges: {
  type client = t

  type exchangeIO = Wonka.Types.sourceT<Types.operation> => Wonka.Types.sourceT<
    Types.operationResultJs<Js.Json.t>,
  >

  type exchangeInput = {
    forward: exchangeIO,
    client: client,
  }

  type t = (
    exchangeInput,
    . Wonka.Types.sourceT<Types.operation>,
  ) => Wonka.Types.sourceT<Types.operationResultJs<Js.Json.t>>

  @module("urql") external cacheExchange: t = "cacheExchange"
  @module("urql") external debugExchange: t = "debugExchange"
  @module("urql") external dedupExchange: t = "dedupExchange"
  @module("urql")
  external fallbackExchangeIO: exchangeIO = "fallbackExchangeIO"
  @module("urql") external fetchExchange: t = "fetchExchange"
  @module("urql")
  external composeExchanges: array<t> => t = "composeExchanges"
  @module("urql")
  external defaultExchanges: array<t> = "defaultExchanges"

  /* Specific types for the subscriptionExchange. */
  type observerLike<'value> = {
    next: 'value => unit,
    error: Js.Exn.t => unit,
    complete: unit => unit,
  }

  type unsubscribe = {unsubscribe: unit => unit}

  type observableLike<'value> = {subscribe: observerLike<'value> => unsubscribe}

  type subscriptionOperation = {
    query: string,
    variables: option<Js.Json.t>,
    key: string,
    context: Types.operationContext,
  }

  type subscriptionForwarder = subscriptionOperation => observableLike<Types.executionResult>

  let subscriptionExchange: (
    ~forwardSubscription: subscriptionForwarder,
    ~enableAllOperations: bool=?,
    unit,
  ) => t

  /* Specific types for the ssrExchange. */
  type serializedError = {
    networkError: option<string>,
    graphQLErrors: array<string>,
  }

  type serializedResult = {
    data: option<Js.Json.t>,
    error: option<serializedError>,
  }

  type ssrExchangeParams = {
    isClient: option<bool>,
    initialState: option<Js.Dict.t<serializedResult>>,
  }

  @send
  external restoreData: (~exchange: t, ~restore: Js.Json.t) => Js.Json.t = "restoreData"
  @send external extractData: (~exchange: t) => Js.Json.t = "extractData"
  @module("urql")
  external ssrExchange: (~ssrExchangeParams: ssrExchangeParams=?, unit) => t = "ssrExchange"

  /* Ecosystem exchanges. */
  @module("@urql/exchange-multipart-fetch")
  external multipartFetchExchange: t = "multipartFetchExchange"

  type persistedFetchExchangeOptions<'a> = {
    preferGetForPersistedQueries: option<bool>,
    generateHash: option<(string, 'a) => Js.Promise.t<string>>,
  }

  let makePersistedFetchExchangeOptions: (
    ~preferGetForPersistedQueries: bool=?,
    ~generateHash: (string, 'a) => Js.Promise.t<string>=?,
    unit,
  ) => persistedFetchExchangeOptions<'a>

  @module("@urql/exchange-persisted-fetch")
  external persistedFetchExchange: persistedFetchExchangeOptions<'a> => t = "persistedFetchExchange"

  @module("@urql/exchange-refocus")
  external refocusExchange: unit => t = "refocusExchange"

  type requestPolicyExchangeOptions = {
    shouldUpgrade: option<Types.operation => bool>,
    ttl: option<int>,
  }

  let makeRequestPolicyExchangeOptions: (
    ~shouldUpgrade: Types.operation => bool=?,
    ~ttl: int=?,
    unit,
  ) => requestPolicyExchangeOptions

  @module("@urql/exchange-request-policy")
  external requestPolicyExchange: requestPolicyExchangeOptions => t = "requestPolicyExchange"

  type retryExchangeOptions = {
    initialDelayMs: option<int>,
    maxDelayMs: option<int>,
    randomDelay: option<bool>,
    maxNumberAttempts: option<int>,
    retryIf: option<(CombinedError.t, Types.operation) => bool>,
  }

  let makeRetryExchangeOptions: (
    ~initialDelayMs: int=?,
    ~maxDelayMs: int=?,
    ~randomDelay: bool=?,
    ~maxNumberAttempts: int=?,
    ~retryIf: (CombinedError.t, Types.operation) => bool=?,
    unit,
  ) => retryExchangeOptions

  @module("@urql/exchange-retry")
  external retryExchange: retryExchangeOptions => t = "retryExchange"
}

let make: (
  ~url: string,
  ~fetchOptions: fetchOptions<'a>=?,
  ~fetch: fetchImpl<'a>=?,
  ~exchanges: array<Exchanges.t>=?,
  ~suspense: bool=?,
  ~requestPolicy: Types.requestPolicy=?,
  ~preferGetMethod: bool=?,
  ~maskTypename: bool=?,
  unit,
) => t

let executeQuery: (
  ~client: t,
  ~query: module(Types.Operation with
    type t = 'data
    and type t_variables = 'variables
    and type Raw.t_variables = 'variablesJs
  ),
  ~additionalTypenames: array<string>=?,
  ~fetchOptions: Fetch.requestInit=?,
  ~fetch: (string, Fetch.requestInit) => Js.Promise.t<Fetch.response>=?,
  ~requestPolicy: Types.requestPolicy=?,
  ~url: string=?,
  ~pollInterval: int=?,
  ~meta: Types.operationDebugMeta=?,
  ~suspense: bool=?,
  ~preferGetMethod: bool=?,
  'variables,
) => Wonka.Types.sourceT<Types.operationResult<'data>>

let executeMutation: (
  ~client: t,
  ~mutation: module(Types.Operation with
    type t = 'data
    and type t_variables = 'variables
    and type Raw.t_variables = 'variablesJs
  ),
  ~additionalTypenames: array<string>=?,
  ~fetchOptions: Fetch.requestInit=?,
  ~fetch: (string, Fetch.requestInit) => Js.Promise.t<Fetch.response>=?,
  ~requestPolicy: Types.requestPolicy=?,
  ~url: string=?,
  ~pollInterval: int=?,
  ~meta: Types.operationDebugMeta=?,
  ~suspense: bool=?,
  ~preferGetMethod: bool=?,
  'variables,
) => Wonka.Types.sourceT<Types.operationResult<'data>>

let executeSubscription: (
  ~client: t,
  ~subscription: module(Types.Operation with
    type t = 'data
    and type t_variables = 'variables
    and type Raw.t_variables = 'variablesJs
  ),
  ~additionalTypenames: array<string>=?,
  ~fetchOptions: Fetch.requestInit=?,
  ~fetch: (string, Fetch.requestInit) => Js.Promise.t<Fetch.response>=?,
  ~requestPolicy: Types.requestPolicy=?,
  ~url: string=?,
  ~pollInterval: int=?,
  ~meta: Types.operationDebugMeta=?,
  ~suspense: bool=?,
  ~preferGetMethod: bool=?,
  'variables,
) => Wonka.Types.sourceT<Types.operationResult<'data>>

let query: (
  ~client: t,
  ~query: module(Types.Operation with
    type t = 'data
    and type t_variables = 'variables
    and type Raw.t_variables = 'variablesJs
  ),
  ~additionalTypenames: array<string>=?,
  ~fetchOptions: Fetch.requestInit=?,
  ~fetch: (string, Fetch.requestInit) => Js.Promise.t<Fetch.response>=?,
  ~requestPolicy: Types.requestPolicy=?,
  ~url: string=?,
  ~pollInterval: int=?,
  ~meta: Types.operationDebugMeta=?,
  ~suspense: bool=?,
  ~preferGetMethod: bool=?,
  'variables,
) => Js.Promise.t<Types.operationResult<'data>>

let mutation: (
  ~client: t,
  ~mutation: module(Types.Operation with
    type t = 'data
    and type t_variables = 'variables
    and type Raw.t_variables = 'variablesJs
  ),
  ~additionalTypenames: array<string>=?,
  ~fetchOptions: Fetch.requestInit=?,
  ~fetch: (string, Fetch.requestInit) => Js.Promise.t<Fetch.response>=?,
  ~requestPolicy: Types.requestPolicy=?,
  ~url: string=?,
  ~pollInterval: int=?,
  ~meta: Types.operationDebugMeta=?,
  ~suspense: bool=?,
  ~preferGetMethod: bool=?,
  'variables,
) => Js.Promise.t<Types.operationResult<'data>>

let subscription: (
  ~client: t,
  ~subscription: module(Types.Operation with
    type t = 'data
    and type t_variables = 'variables
    and type Raw.t_variables = 'variablesJs
  ),
  ~additionalTypenames: array<string>=?,
  ~fetchOptions: Fetch.requestInit=?,
  ~fetch: (string, Fetch.requestInit) => Js.Promise.t<Fetch.response>=?,
  ~requestPolicy: Types.requestPolicy=?,
  ~url: string=?,
  ~pollInterval: int=?,
  ~meta: Types.operationDebugMeta=?,
  ~suspense: bool=?,
  ~preferGetMethod: bool=?,
  'variables,
) => Wonka.Types.sourceT<Types.operationResult<'data>>

let readQuery: (
  ~client: t,
  ~query: module(Types.Operation with
    type t = 'data
    and type t_variables = 'variables
    and type Raw.t_variables = 'variablesJs
  ),
  ~additionalTypenames: array<string>=?,
  ~fetchOptions: Fetch.requestInit=?,
  ~fetch: (string, Fetch.requestInit) => Js.Promise.t<Fetch.response>=?,
  ~requestPolicy: Types.requestPolicy=?,
  ~url: string=?,
  ~pollInterval: int=?,
  ~meta: Types.operationDebugMeta=?,
  ~suspense: bool=?,
  ~preferGetMethod: bool=?,
  'variables,
) => option<Types.operationResult<'data>>
