type t = {
  url: string,
  suspense: bool,
  preferGetMethod: bool,
  requestPolicy: string,
  maskTypename: bool,
};

/* Helpers for supporting polymorphic fetchOptions. */
type fetchOptions('a) =
  | FetchOpts(Fetch.requestInit): fetchOptions(Fetch.requestInit)
  | FetchFn(unit => Fetch.requestInit)
    : fetchOptions(unit => Fetch.requestInit);

let unwrapFetchOptions =
    (type a, fetchOptions: option(fetchOptions(a))): option(a) =>
  switch (fetchOptions) {
  | Some(FetchOpts(opts)) => Some(opts)
  | Some(FetchFn(fn)) => Some(fn)
  | None => None
  };

/* Helpers for supporting a user-supplied fetch function. */
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

let unwrapFetchImpl = (type a, fetch: option(fetchImpl(a))): option(a) => {
  switch (fetch) {
  | Some(FetchWithUrl(impl)) => Some(impl)
  | Some(FetchWithUrlOptions(impl)) => Some(impl)
  | Some(FetchWithRequest(impl)) => Some(impl)
  | Some(FetchWithRequestOptions(impl)) => Some(impl)
  | None => None
  };
};

module Exchanges = {
  type client = t;

  type exchangeIO =
    Wonka.Types.sourceT(Types.operation) =>
    Wonka.Types.sourceT(Types.operationResultJs(Js.Json.t));

  type exchangeInput = {
    forward: exchangeIO,
    client,
  };

  type t =
    exchangeInput =>
    (. Wonka.Types.sourceT(Types.operation)) =>
    Wonka.Types.sourceT(Types.operationResultJs(Js.Json.t));

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

  type subscriptionExchangeOpts = {
    forwardSubscription: subscriptionForwarder,
    enableAllOperations: option(bool),
  };

  [@bs.module "urql"]
  external subscriptionExchangeJS: subscriptionExchangeOpts => t =
    "subscriptionExchange";

  let subscriptionExchange =
      (~forwardSubscription, ~enableAllOperations=?, ()) => {
    subscriptionExchangeJS({forwardSubscription, enableAllOperations});
  };

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

type clientOptions('fetchOptions, 'fetchImpl) = {
  url: string,
  fetchOptions: option('fetchOptions),
  exchanges: array(Exchanges.t),
  suspense: bool,
  fetch: option('fetchImpl),
  requestPolicy: string,
  preferGetMethod: bool,
  maskTypename: bool,
};

[@bs.new] [@bs.module "urql"]
external client: clientOptions('fetchOptions, 'fetchImpl) => t = "Client";

/* `make` is equivalent to urql's `createClient`.
   We opt to use `make` here to adhere to standards in the Reason community. */
let make =
    (
      ~url,
      ~fetchOptions=?,
      ~fetch=?,
      ~exchanges=Exchanges.defaultExchanges,
      ~suspense=false,
      ~requestPolicy=`CacheFirst,
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
  };

  client(options);
};

/* The record representing the response returned by the client after
   it has been converted by urqlClientResponseToReason. */
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

/* A function to convert the JS response from a client.execute*
   methods to typed a Reason record. */
let clientResponseToReason:
  type data.
    (~response: Types.operationResultJs('a), ~parse: 'a => data) =>
    clientResponse(data) =
  (~response, ~parse) => {
    let {extensions, stale}: Types.operationResultJs('a) = response;
    let data = response.data->Js.Nullable.toOption->Belt.Option.map(parse);
    let error =
      response.error->Belt.Option.map(CombinedError.combinedErrorToRecord);

    let response =
      switch (data, error) {
      | (Some(data), _) => Data(data)
      | (None, Some(error)) => Error(error)
      | (None, None) => Empty
      };

    {data, error, extensions, stale, response};
  };

/* Execution methods on the client. These allow you to imperatively execute GraphQL
   operations outside of the provided hooks. */
[@bs.send]
external executeQueryJs:
  (
    ~client: t,
    ~query: Types.graphqlRequest,
    ~opts: Types.partialOperationContext=?,
    unit
  ) =>
  Wonka.Types.sourceT(Types.operationResultJs('data)) =
  "executeQuery";

let executeQuery:
  type variables variablesJs data.
    (
      ~client: t,
      ~request: (module Types.Operation with
                   type t_variables = variables and
                   type Raw.t_variables = variablesJs and
                   type t = data),
      ~variables: variables,
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
    Wonka.Types.sourceT(clientResponse(data)) =
  (
    ~client,
    ~request as (module Request),
    ~variables: variables,
    ~additionalTypenames=?,
    ~fetchOptions=?,
    ~fetch=?,
    ~requestPolicy=?,
    ~url=?,
    ~pollInterval=?,
    ~meta=?,
    ~suspense=?,
    ~preferGetMethod=?,
    (),
  ) => {
    let req =
      Utils.createRequest(
        ~query=Request.query,
        ~variables=
          variables->Request.serializeVariables->Request.variablesToJson,
        (),
      );

    let optsJs =
      Types.partialOperationContext(
        ~additionalTypenames?,
        ~fetchOptions?,
        ~fetch?,
        ~requestPolicy=?
          requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
        ~url?,
        ~pollInterval?,
        ~meta?,
        ~suspense?,
        ~preferGetMethod?,
        (),
      );

    executeQueryJs(~client, ~query=req, ~opts=optsJs, ())
    |> Wonka.map((. response) =>
         clientResponseToReason(~response, ~parse=Request.parse)
       );
  };

[@bs.send]
external executeMutationJs:
  (
    ~client: t,
    ~mutation: Types.graphqlRequest,
    ~opts: Types.partialOperationContext=?,
    unit
  ) =>
  Wonka.Types.sourceT(Types.operationResultJs('data)) =
  "executeMutation";

let executeMutation:
  type variables variablesJs data.
    (
      ~client: t,
      ~request: (module Types.Operation with
                   type t_variables = variables and
                   type Raw.t_variables = variablesJs and
                   type t = data),
      ~variables: variables,
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
    Wonka.Types.sourceT(clientResponse(data)) =
  (
    ~client: t,
    ~request as (module Request),
    ~variables,
    ~additionalTypenames=?,
    ~fetchOptions=?,
    ~fetch=?,
    ~requestPolicy=?,
    ~url=?,
    ~pollInterval=?,
    ~meta=?,
    ~suspense=?,
    ~preferGetMethod=?,
    (),
  ) => {
    let req =
      Utils.createRequest(
        ~query=Request.query,
        ~variables=
          variables->Request.serializeVariables->Request.variablesToJson,
        (),
      );
    let parse = Request.parse;
    let optsJs =
      Types.partialOperationContext(
        ~additionalTypenames?,
        ~fetchOptions?,
        ~fetch?,
        ~requestPolicy=?
          requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
        ~url?,
        ~pollInterval?,
        ~meta?,
        ~suspense?,
        ~preferGetMethod?,
        (),
      );

    executeMutationJs(~client, ~mutation=req, ~opts=optsJs, ())
    |> Wonka.map((. response) => clientResponseToReason(~response, ~parse));
  };

[@bs.send]
external executeSubscriptionJs:
  (
    ~client: t,
    ~subscription: Types.graphqlRequest,
    ~opts: Types.partialOperationContext=?,
    unit
  ) =>
  Wonka.Types.sourceT(Types.operationResultJs('data)) =
  "executeSubscription";

let executeSubscription:
  type variables variablesJs data.
    (
      ~client: t,
      ~request: (module Types.Operation with
                   type t_variables = variables and
                   type Raw.t_variables = variablesJs and
                   type t = data),
      ~variables: variables,
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
    Wonka.Types.sourceT(clientResponse(data)) =
  (
    ~client: t,
    ~request as (module Request),
    ~variables,
    ~additionalTypenames=?,
    ~fetchOptions=?,
    ~fetch=?,
    ~requestPolicy=?,
    ~url=?,
    ~pollInterval=?,
    ~meta=?,
    ~suspense=?,
    ~preferGetMethod=?,
    (),
  ) => {
    let req =
      Utils.createRequest(
        ~query=Request.query,
        ~variables=
          variables->Request.serializeVariables->Request.variablesToJson,
        (),
      );
    let parse = Request.parse;
    let optsJs =
      Types.partialOperationContext(
        ~additionalTypenames?,
        ~fetchOptions?,
        ~fetch?,
        ~requestPolicy=?
          requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
        ~url?,
        ~pollInterval?,
        ~meta?,
        ~suspense?,
        ~preferGetMethod?,
        (),
      );

    executeSubscriptionJs(~client, ~subscription=req, ~opts=optsJs, ())
    |> Wonka.map((. response) => clientResponseToReason(~response, ~parse));
  };

let query =
    (
      ~client,
      ~request,
      ~variables,
      ~additionalTypenames=?,
      ~fetchOptions=?,
      ~fetch=?,
      ~requestPolicy=?,
      ~url=?,
      ~pollInterval=?,
      ~meta=?,
      ~suspense=?,
      ~preferGetMethod=?,
      (),
    ) => {
  executeQuery(
    ~client,
    ~request,
    ~variables,
    ~additionalTypenames?,
    ~fetchOptions?,
    ~fetch?,
    ~requestPolicy?,
    ~url?,
    ~pollInterval?,
    ~meta?,
    ~suspense?,
    ~preferGetMethod?,
    (),
  )
  |> Wonka.take(1)
  |> Wonka.toPromise;
};

let mutation =
    (
      ~client,
      ~request,
      ~variables,
      ~additionalTypenames=?,
      ~fetchOptions=?,
      ~fetch=?,
      ~requestPolicy=?,
      ~url=?,
      ~pollInterval=?,
      ~meta=?,
      ~suspense=?,
      ~preferGetMethod=?,
      (),
    ) => {
  executeMutation(
    ~client,
    ~request,
    ~variables,
    ~additionalTypenames?,
    ~fetchOptions?,
    ~fetch?,
    ~requestPolicy?,
    ~url?,
    ~pollInterval?,
    ~meta?,
    ~suspense?,
    ~preferGetMethod?,
    (),
  )
  |> Wonka.take(1)
  |> Wonka.toPromise;
};

let subscription = executeSubscription;

let readQuery =
    (
      ~client,
      ~request,
      ~variables,
      ~additionalTypenames=?,
      ~fetchOptions=?,
      ~fetch=?,
      ~requestPolicy=?,
      ~url=?,
      ~pollInterval=?,
      ~meta=?,
      ~suspense=?,
      ~preferGetMethod=?,
      (),
    ) => {
  let result = ref(None);
  executeQuery(
    ~client,
    ~request,
    ~variables,
    ~additionalTypenames?,
    ~fetchOptions?,
    ~fetch?,
    ~requestPolicy?,
    ~url?,
    ~pollInterval?,
    ~meta?,
    ~suspense?,
    ~preferGetMethod?,
    (),
  )
  |> Wonka.subscribe((. data) => {result := Some(data)})
  |> (subscription => subscription.unsubscribe());
  result^;
};
