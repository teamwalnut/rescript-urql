type t = {
  url: string,
  suspense: bool,
  preferGetMethod: bool,
  requestPolicy: string,
  maskTypename: bool,
}

/* Helpers for supporting polymorphic fetchOptions. */
type rec fetchOptions<'a> =
  | FetchOpts(Fetch.Request.init): fetchOptions<Fetch.Request.init>
  | FetchFn(unit => Fetch.Request.init): fetchOptions<unit => Fetch.Request.init>

let unwrapFetchOptions = (type a, fetchOptions: option<fetchOptions<a>>): option<a> =>
  switch fetchOptions {
  | Some(FetchOpts(opts)) => Some(opts)
  | Some(FetchFn(fn)) => Some(fn)
  | None => None
  }

/* Helpers for supporting a user-supplied fetch function. */
type rec fetchImpl<'a> =
  | FetchWithUrl(string => Js.Promise.t<Fetch.Response.t>): fetchImpl<
      string => Js.Promise.t<Fetch.Response.t>,
    >
  | FetchWithUrlOptions((string, Fetch.Request.init) => Js.Promise.t<Fetch.Response.t>): fetchImpl<
      (string, Fetch.Request.init) => Js.Promise.t<Fetch.Response.t>,
    >
  | FetchWithRequest(Fetch.Request.t => Js.Promise.t<Fetch.Response.t>): fetchImpl<
      Fetch.Request.t => Js.Promise.t<Fetch.Response.t>,
    >
  | FetchWithRequestOptions(
      (Fetch.Request.t, Fetch.Request.init) => Js.Promise.t<Fetch.Response.t>,
    ): fetchImpl<(Fetch.Request.t, Fetch.Request.init) => Js.Promise.t<Fetch.Response.t>>

let unwrapFetchImpl = (type a, fetch: option<fetchImpl<a>>): option<a> =>
  switch fetch {
  | Some(FetchWithUrl(impl)) => Some(impl)
  | Some(FetchWithUrlOptions(impl)) => Some(impl)
  | Some(FetchWithRequest(impl)) => Some(impl)
  | Some(FetchWithRequestOptions(impl)) => Some(impl)
  | None => None
  }

module Exchanges = {
  type client = t

  type exchangeIO = Wonka.Source.t<Types.operation> => Wonka.Source.t<
    Types.operationResultJs<Js.Json.t>,
  >

  type exchangeInput = {
    forward: exchangeIO,
    client: client,
  }

  type t = exchangeInput => (
    . Wonka.Source.t<Types.operation>,
  ) => Wonka.Source.t<Types.operationResultJs<Js.Json.t>>

  @module("@urql/core") external cacheExchange: t = "cacheExchange"
  @module("@urql/core") external debugExchange: t = "debugExchange"
  @module("@urql/core") external dedupExchange: t = "dedupExchange"
  @module("@urql/core")
  external fallbackExchangeIO: exchangeIO = "fallbackExchangeIO"
  @module("@urql/core") external fetchExchange: t = "fetchExchange"
  @module("@urql/core")
  external composeExchanges: array<t> => t = "composeExchanges"
  @module("@urql/core")
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

  type subscriptionExchangeOpts = {
    forwardSubscription: subscriptionForwarder,
    enableAllOperations: option<bool>,
  }

  @module("urql")
  external subscriptionExchangeJS: subscriptionExchangeOpts => t = "subscriptionExchange"

  let subscriptionExchange = (~forwardSubscription, ~enableAllOperations=?, ()) =>
    subscriptionExchangeJS({
      forwardSubscription,
      enableAllOperations,
    })

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

  @module("@urql/core")
  external ssrExchange: (~ssrExchangeParams: ssrExchangeParams=?, unit) => t = "ssrExchange"

  /* Ecosystem exchanges. */
  @module("@urql/exchange-multipart-fetch")
  external multipartFetchExchange: t = "multipartFetchExchange"

  type persistedFetchExchangeOptions<'a> = {
    preferGetForPersistedQueries: option<bool>,
    generateHash: option<(string, 'a) => Js.Promise.t<string>>,
  }

  let makePersistedFetchExchangeOptions = (
    ~preferGetForPersistedQueries=?,
    ~generateHash=?,
    (),
  ) => {
    preferGetForPersistedQueries,
    generateHash,
  }

  @module("@urql/exchange-persisted-fetch")
  external persistedFetchExchange: persistedFetchExchangeOptions<'a> => t = "persistedFetchExchange"

  @module("@urql/exchange-refocus")
  external refocusExchange: unit => t = "refocusExchange"

  type requestPolicyExchangeOptions = {
    shouldUpgrade: option<Types.operation => bool>,
    ttl: option<int>,
  }

  let makeRequestPolicyExchangeOptions = (~shouldUpgrade=?, ~ttl=?, ()) => {
    shouldUpgrade,
    ttl,
  }

  @module("@urql/exchange-request-policy")
  external requestPolicyExchange: requestPolicyExchangeOptions => t = "requestPolicyExchange"

  type retryExchangeOptions = {
    initialDelayMs: option<int>,
    maxDelayMs: option<int>,
    randomDelay: option<bool>,
    maxNumberAttempts: option<int>,
    retryIf: option<(CombinedError.t, Types.operation) => bool>,
  }

  let makeRetryExchangeOptions = (
    ~initialDelayMs=?,
    ~maxDelayMs=?,
    ~randomDelay=?,
    ~maxNumberAttempts=?,
    ~retryIf=?,
    (),
  ) => {
    initialDelayMs,
    maxDelayMs,
    randomDelay,
    maxNumberAttempts,
    retryIf,
  }

  @module("@urql/exchange-retry")
  external retryExchange: retryExchangeOptions => t = "retryExchange"
}

type clientOptions<'fetchOptions, 'fetchImpl> = {
  url: string,
  fetchOptions: option<'fetchOptions>,
  exchanges: array<Exchanges.t>,
  suspense: bool,
  fetch: option<'fetchImpl>,
  requestPolicy: string,
  preferGetMethod: bool,
  maskTypename: bool,
}

@new @module("@urql/core")
external client: clientOptions<'fetchOptions, 'fetchImpl> => t = "Client"

/* `make` is equivalent to urql's `createClient`.
 We opt to use `make` here to adhere to standards in the ReScript community. */
let make = (
  ~url,
  ~fetchOptions=?,
  ~fetch=?,
  ~exchanges=Exchanges.defaultExchanges,
  ~suspense=false,
  ~requestPolicy=#"cache-first",
  ~preferGetMethod=false,
  ~maskTypename=false,
  (),
) => {
  let options = {
    url,
    fetchOptions: fetchOptions->unwrapFetchOptions,
    fetch: fetch->unwrapFetchImpl,
    exchanges,
    suspense,
    requestPolicy: requestPolicy->Types.requestPolicyToJs,
    preferGetMethod,
    maskTypename,
  }

  client(options)
}

/* Execution methods on the client. These allow you to imperatively execute GraphQL
 operations outside of the provided hooks. */
@send
external executeQueryJs: (
  ~client: t,
  ~query: Types.graphqlRequest,
  ~opts: Types.partialOperationContext=?,
  unit,
) => Wonka.Source.t<Types.operationResultJs<'data>> = "executeQuery"

let executeQuery:
  type variables variablesJs data. (
    ~client: t,
    ~query: module(Types.Operation with
      type t_variables = variables
      and type Raw.t_variables = variablesJs
      and type t = data
    ),
    ~additionalTypenames: array<string>=?,
    ~fetchOptions: Fetch.Request.init=?,
    ~fetch: (string, Fetch.Request.init) => Js.Promise.t<Fetch.Response.t>=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~url: string=?,
    ~meta: Types.operationDebugMeta=?,
    ~suspense: bool=?,
    ~preferGetMethod: bool=?,
    variables,
  ) => Wonka.Source.t<Types.operationResult<data>> =
  (
    ~client,
    ~query as module(Query),
    ~additionalTypenames=?,
    ~fetchOptions=?,
    ~fetch=?,
    ~requestPolicy=?,
    ~url=?,
    ~meta=?,
    ~suspense=?,
    ~preferGetMethod=?,
    variables,
  ) => {
    let req = Utils.createRequest(
      ~query=Query.query,
      ~variables=variables->Query.serializeVariables->Query.variablesToJson,
      (),
    )

    let optsJs = Types.partialOperationContext(
      ~additionalTypenames?,
      ~fetchOptions?,
      ~fetch?,
      ~requestPolicy=?requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
      ~url?,
      ~meta?,
      ~suspense?,
      ~preferGetMethod?,
      (),
    )

    executeQueryJs(~client, ~query=req, ~opts=optsJs, ()) |> Wonka.map((. response) =>
      Types.operationResultToReScript(~response, ~parse=Query.parse)
    )
  }

@send
external executeMutationJs: (
  ~client: t,
  ~mutation: Types.graphqlRequest,
  ~opts: Types.partialOperationContext=?,
  unit,
) => Wonka.Source.t<Types.operationResultJs<'data>> = "executeMutation"

let executeMutation:
  type variables variablesJs data. (
    ~client: t,
    ~mutation: module(Types.Operation with
      type t_variables = variables
      and type Raw.t_variables = variablesJs
      and type t = data
    ),
    ~additionalTypenames: array<string>=?,
    ~fetchOptions: Fetch.Request.init=?,
    ~fetch: (string, Fetch.Request.init) => Js.Promise.t<Fetch.Response.t>=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~url: string=?,
    ~meta: Types.operationDebugMeta=?,
    ~suspense: bool=?,
    ~preferGetMethod: bool=?,
    variables,
  ) => Wonka.Source.t<Types.operationResult<data>> =
  (
    ~client: t,
    ~mutation as module(Mutation),
    ~additionalTypenames=?,
    ~fetchOptions=?,
    ~fetch=?,
    ~requestPolicy=?,
    ~url=?,
    ~meta=?,
    ~suspense=?,
    ~preferGetMethod=?,
    variables,
  ) => {
    let req = Utils.createRequest(
      ~query=Mutation.query,
      ~variables=variables->Mutation.serializeVariables->Mutation.variablesToJson,
      (),
    )
    let optsJs = Types.partialOperationContext(
      ~additionalTypenames?,
      ~fetchOptions?,
      ~fetch?,
      ~requestPolicy=?requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
      ~url?,
      ~meta?,
      ~suspense?,
      ~preferGetMethod?,
      (),
    )

    executeMutationJs(~client, ~mutation=req, ~opts=optsJs, ()) |> Wonka.map((. response) =>
      Types.operationResultToReScript(~response, ~parse=Mutation.parse)
    )
  }

@send
external executeSubscriptionJs: (
  ~client: t,
  ~subscription: Types.graphqlRequest,
  ~opts: Types.partialOperationContext=?,
  unit,
) => Wonka.Source.t<Types.operationResultJs<'data>> = "executeSubscription"

let executeSubscription:
  type variables variablesJs data. (
    ~client: t,
    ~subscription: module(Types.Operation with
      type t_variables = variables
      and type Raw.t_variables = variablesJs
      and type t = data
    ),
    ~additionalTypenames: array<string>=?,
    ~fetchOptions: Fetch.Request.init=?,
    ~fetch: (string, Fetch.Request.init) => Js.Promise.t<Fetch.Response.t>=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~url: string=?,
    ~meta: Types.operationDebugMeta=?,
    ~suspense: bool=?,
    ~preferGetMethod: bool=?,
    variables,
  ) => Wonka.Source.t<Types.operationResult<data>> =
  (
    ~client: t,
    ~subscription as module(Subscription),
    ~additionalTypenames=?,
    ~fetchOptions=?,
    ~fetch=?,
    ~requestPolicy=?,
    ~url=?,
    ~meta=?,
    ~suspense=?,
    ~preferGetMethod=?,
    variables,
  ) => {
    let req = Utils.createRequest(
      ~query=Subscription.query,
      ~variables=variables->Subscription.serializeVariables->Subscription.variablesToJson,
      (),
    )
    let optsJs = Types.partialOperationContext(
      ~additionalTypenames?,
      ~fetchOptions?,
      ~fetch?,
      ~requestPolicy=?requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
      ~url?,
      ~meta?,
      ~suspense?,
      ~preferGetMethod?,
      (),
    )

    executeSubscriptionJs(~client, ~subscription=req, ~opts=optsJs, ()) |> Wonka.map((. response) =>
      Types.operationResultToReScript(~response, ~parse=Subscription.parse)
    )
  }

let query = (
  ~client,
  ~query,
  ~additionalTypenames=?,
  ~fetchOptions=?,
  ~fetch=?,
  ~requestPolicy=?,
  ~url=?,
  ~meta=?,
  ~suspense=?,
  ~preferGetMethod=?,
  variables,
) =>
  executeQuery(
    ~client,
    ~query,
    ~additionalTypenames?,
    ~fetchOptions?,
    ~fetch?,
    ~requestPolicy?,
    ~url?,
    ~meta?,
    ~suspense?,
    ~preferGetMethod?,
    variables,
  )
  |> Wonka.take(1)
  |> Wonka.toPromise

let mutation = (
  ~client,
  ~mutation,
  ~additionalTypenames=?,
  ~fetchOptions=?,
  ~fetch=?,
  ~requestPolicy=?,
  ~url=?,
  ~meta=?,
  ~suspense=?,
  ~preferGetMethod=?,
  variables,
) =>
  executeMutation(
    ~client,
    ~mutation,
    ~additionalTypenames?,
    ~fetchOptions?,
    ~fetch?,
    ~requestPolicy?,
    ~url?,
    ~meta?,
    ~suspense?,
    ~preferGetMethod?,
    variables,
  )
  |> Wonka.take(1)
  |> Wonka.toPromise

let subscription = executeSubscription

let readQuery = (
  ~client,
  ~query,
  ~additionalTypenames=?,
  ~fetchOptions=?,
  ~fetch=?,
  ~requestPolicy=?,
  ~url=?,
  ~meta=?,
  ~suspense=?,
  ~preferGetMethod=?,
  variables,
) => {
  let result = ref(None)
  executeQuery(
    ~client,
    ~query,
    ~additionalTypenames?,
    ~fetchOptions?,
    ~fetch?,
    ~requestPolicy?,
    ~url?,
    ~meta?,
    ~suspense?,
    ~preferGetMethod?,
    variables,
  )
  |> Wonka.subscribe((. data) => result := Some(data))
  |> (subscription => subscription.unsubscribe())
  result.contents
}
