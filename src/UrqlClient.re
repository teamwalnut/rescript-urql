type t;

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

/* Types used and exposed by the urql client. */
module ClientTypes = {
  /* The result of executing a GraphQL request.
     Consists of optional data and errors fields. */
  [@bs.deriving abstract]
  type executionResult = {
    errors: Js.Nullable.t(array(UrqlCombinedError.graphQLError)),
    data: Js.Nullable.t(Js.Json.t),
  };

  /* OperationType for the active operation.
     Use with operationTypeToJs for proper conversion to strings. */
  [@bs.deriving jsConverter]
  type operationType = [
    | [@bs.as "subscription"] `Subscription
    | [@bs.as "query"] `Query
    | [@bs.as "mutation"] `Mutation
    | [@bs.as "teardown"] `Teardown
  ];

  /* Additional operation metadata to pass to the active operation.
     We currently do not support additional untyped options. */

  /* Cache outcomes for operations. This is likely to be deprecated
     in future releases. */
  [@bs.deriving jsConverter]
  type cacheOutcome = [
    | [@bs.as "miss"] `Miss
    | [@bs.as "partial"] `Partial
    | [@bs.as "hit"] `Hit
  ];

  /* Debug information on specific operations. */
  [@bs.deriving abstract]
  type operationDebugMeta = {
    [@bs.optional]
    cacheOutcome,
  };

  /* The operation context object for a request. */
  [@bs.deriving abstract]
  type operationContext = {
    [@bs.optional]
    fetchOptions: Fetch.requestInit,
    requestPolicy: UrqlTypes.requestPolicy,
    url: string,
    [@bs.optional]
    meta: operationDebugMeta,
    [@bs.optional]
    pollInterval: int,
  };

  /* A partial operation context, which can be passed as the
     second optional argument to executeQuery and executeMutation.
     Simply makes all properties on operationContext optional. */
  [@bs.deriving abstract]
  type partialOperationContext = {
    [@bs.optional] [@bs.as "fetchOptions"]
    partialOpFetchOptions: Fetch.requestInit,
    [@bs.optional] [@bs.as "requestPolicy"]
    partialOpRequestPolicy: UrqlTypes.requestPolicy,
    [@bs.optional] [@bs.as "url"]
    partialOpUrl: string,
    [@bs.optional] [@bs.as "meta"]
    partialOpDebugMeta: operationDebugMeta,
    [@bs.optional] [@bs.as "pollInterval"]
    partialOpPollInterval: int,
  };

  [@bs.deriving abstract]
  type partialOperationContextJs = {
    [@bs.optional] [@bs.as "fetchOptions"]
    partialOpFetchOptionsJs: Fetch.requestInit,
    [@bs.optional] [@bs.as "requestPolicy"]
    partialOpRequestPolicyJs: string,
    [@bs.optional] [@bs.as "url"]
    partialOpUrlJs: string,
    [@bs.optional] [@bs.as "meta"]
    partialOpDebugMetaJs: operationDebugMeta,
    [@bs.optional] [@bs.as "pollInterval"]
    partialOpPollIntervalJs: int,
  };

  /* The active GraphQL operation. */
  [@bs.deriving abstract]
  type operation = {
    key: int,
    query: string,
    [@bs.optional]
    variables: Js.Json.t,
    operationName: operationType,
    context: operationContext,
  };

  /* The result of the GraphQL operation. */
  [@bs.deriving abstract]
  type operationResult = {
    operation,
    data: Js.Nullable.t(Js.Json.t),
    error: Js.Nullable.t(UrqlCombinedError.combinedErrorJs),
  };

  /* The record representing the response returned by the client _after_
     it has been converted by urqlClientResponseToReason. */
  type response('response) =
    | Data('response)
    | Error(UrqlCombinedError.t)
    | NotFound;

  type clientResponse('response) = {
    data: option('response),
    error: option(UrqlCombinedError.t),
    response: response('response),
  };
};

/* A module for binding exchange types and urql's exposed exchanges. Since this module
   references UrqlClient and UrqlClient references this module,
   they need to be co-located to avoid a cyclic dependency error. */
module UrqlExchanges = {
  type exchangeIO =
    Wonka.Types.sourceT(ClientTypes.operation) =>
    Wonka.Types.sourceT(ClientTypes.operationResult);

  [@bs.deriving abstract]
  type exchangeInput = {
    forward: exchangeIO,
    client: t,
  };

  type exchange = exchangeInput => exchangeIO;

  [@bs.module "urql"] external cacheExchange: exchange = "cacheExchange";
  [@bs.module "urql"] external debugExchange: exchange = "debugExchange";
  [@bs.module "urql"] external dedupExchange: exchange = "dedupExchange";
  [@bs.module "urql"]
  external fallbackExchangeIO: exchangeIO = "fallbackExchangeIO";
  [@bs.module "urql"] external fetchExchange: exchange = "fetchExchange";
  [@bs.module "urql"]
  external composeExchanges: array(exchange) => exchange = "composeExchanges";
  [@bs.module "urql"]
  external defaultExchanges: array(exchange) = "defaultExchanges";

  /* Specific types for the subscriptionExchange. */
  [@bs.deriving abstract]
  type observerLike('value) = {
    next: 'value => unit,
    error: Js.Exn.t => unit,
    complete: unit => unit,
  };

  [@bs.deriving abstract]
  type observableLike('value) = {
    subscribe:
      observerLike('value) => {. [@bs.meth] "unsubscribe": unit => unit},
  };

  [@bs.deriving abstract]
  type subscriptionOperation = {
    query: string,
    [@bs.optional]
    variables: Js.Json.t,
    key: string,
    context: ClientTypes.operationContext,
  };

  type subscriptionForwarder =
    subscriptionOperation => observableLike(ClientTypes.executionResult);

  [@bs.deriving abstract]
  type subscriptionExchangeOpts = {
    forwardSubscription: subscriptionForwarder,
  };

  [@bs.module "urql"]
  external subscriptionExchange: subscriptionExchangeOpts => exchange =
    "subscriptionExchange";

  /* Specific types for the ssrExchange. */
  [@bs.deriving abstract]
  type serializedError = {
    networkError: Js.Nullable.t(string),
    graphQLErrors: Js.Nullable.t(array(string)),
  };

  [@bs.deriving abstract]
  type serializedResult = {
    [@bs.optional]
    data: Js.Json.t,
    [@bs.optional]
    error: serializedError,
  };

  [@bs.deriving abstract]
  type ssrExchangeOpts = {
    initialState: Js.Dict.t(serializedResult),
    [@bs.optional]
    isClient: bool,
  };

  [@bs.send]
  external restoreData: (~exchange: exchange, ~restore: Js.Json.t) => Js.Json.t =
    "restoreData";
  [@bs.send]
  external extractData: (~exchange: exchange) => Js.Json.t = "extractData";

  [@bs.module "urql"]
  external ssrExchange: (~ssrExchangeOpts: ssrExchangeOpts=?, unit) => exchange =
    "ssrExchange";
};

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

[@bs.deriving abstract]
type clientOptions('a, 'b) = {
  url: string,
  [@bs.optional]
  fetchOptions: 'a,
  [@bs.optional]
  exchanges: array(UrqlExchanges.exchange),
  [@bs.optional]
  suspense: bool,
  [@bs.optional]
  fetch: 'b,
  [@bs.optional]
  requestPolicy: string,
};

[@bs.new] [@bs.module "urql"]
external client: clientOptions('a, 'b) => t = "Client";

/* `make` is equivalent to urql's `createClient`.
   We opt to use `make` here to adhere to standards in the Reason community. */
let make =
    (
      ~url,
      ~fetchOptions=?,
      ~requestPolicy=`CacheFirst,
      ~exchanges=[|
                   UrqlExchanges.dedupExchange,
                   UrqlExchanges.cacheExchange,
                   UrqlExchanges.fetchExchange,
                 |],
      ~suspense=false,
      ~fetch=?,
      (),
    ) => {
  let options =
    clientOptions(
      ~url,
      ~fetchOptions=unwrapFetchOptions(fetchOptions),
      ~exchanges,
      ~suspense,
      ~fetch=unwrapFetchImpl(fetch),
      ~requestPolicy=requestPolicy->UrqlTypes.requestPolicyToJs,
      (),
    );

  client(options);
};

/* A function to convert the JS response from the client to a Reason record. */
let urqlClientResponseToReason =
    (~parse: Js.Json.t => 'response, ~result: ClientTypes.operationResult)
    : ClientTypes.clientResponse('response) => {
  let data =
    result->ClientTypes.dataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->ClientTypes.errorGet
    ->Js.Nullable.toOption
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);

  let response =
    switch (data, error) {
    | (Some(data), _) => ClientTypes.Data(data)
    | (None, Some(error)) => Error(error)
    | (None, None) => NotFound
    };

  {data, error, response};
};

/* A function to convert a partialOperationContext to a partialOperationContextJs. */
let partialOpCtxToPartialOpCtxJs = opts =>
  switch (opts) {
  | Some(o) =>
    let url = o->ClientTypes.partialOpUrlGet;
    let pollInterval = o->ClientTypes.partialOpPollIntervalGet;
    let fetchOptions = o->ClientTypes.partialOpFetchOptionsGet;
    let requestPolicy =
      o
      ->ClientTypes.partialOpRequestPolicyGet
      ->Belt.Option.map(UrqlTypes.requestPolicyToJs);
    let debugMeta = o->ClientTypes.partialOpDebugMetaGet;

    Some(
      ClientTypes.partialOperationContextJs(
        ~partialOpUrlJs=?url,
        ~partialOpFetchOptionsJs=?fetchOptions,
        ~partialOpRequestPolicyJs=?requestPolicy,
        ~partialOpDebugMetaJs=?debugMeta,
        ~partialOpPollIntervalJs=?pollInterval,
        (),
      ),
    );
  | None => None
  };

/* Execution methods on the client. These allow you to imperatively execute GraphQL
   operations outside of components or hooks. */
[@bs.send]
external executeQueryJs:
  (
    ~client: t,
    ~query: UrqlTypes.graphqlRequest,
    ~opts: ClientTypes.partialOperationContextJs=?,
    unit
  ) =>
  Wonka.Types.sourceT(ClientTypes.operationResult) =
  "executeQuery";

let executeQuery =
    (
      ~client: t,
      ~request: UrqlTypes.request('response),
      ~opts: option(ClientTypes.partialOperationContext)=?,
      (),
    )
    : Wonka.Types.sourceT(ClientTypes.clientResponse('response)) => {
  let req =
    UrqlRequest.createRequest(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );
  let parse = request##parse;
  let optsJs = partialOpCtxToPartialOpCtxJs(opts);

  executeQueryJs(~client, ~query=req, ~opts=?optsJs, ())
  |> Wonka.map((. result) => urqlClientResponseToReason(~parse, ~result));
};

[@bs.send]
external executeMutationJs:
  (
    ~client: t,
    ~mutation: UrqlTypes.graphqlRequest,
    ~opts: ClientTypes.partialOperationContextJs=?,
    unit
  ) =>
  Wonka.Types.sourceT(ClientTypes.operationResult) =
  "executeMutation";

let executeMutation =
    (
      ~client: t,
      ~request: UrqlTypes.request('response),
      ~opts: option(ClientTypes.partialOperationContext)=?,
      (),
    )
    : Wonka.Types.sourceT(ClientTypes.clientResponse('response)) => {
  let req =
    UrqlRequest.createRequest(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );
  let parse = request##parse;
  let optsJs = partialOpCtxToPartialOpCtxJs(opts);

  executeMutationJs(~client, ~mutation=req, ~opts=?optsJs, ())
  |> Wonka.map((. result) => urqlClientResponseToReason(~parse, ~result));
};

[@bs.send]
external executeSubscriptionJs:
  (
    ~client: t,
    ~subscription: UrqlTypes.graphqlRequest,
    ~opts: ClientTypes.partialOperationContextJs=?,
    unit
  ) =>
  Wonka.Types.sourceT(ClientTypes.operationResult) =
  "executeSubscription";

let executeSubscription =
    (
      ~client: t,
      ~request: UrqlTypes.request('response),
      ~opts: option(ClientTypes.partialOperationContext)=?,
      (),
    )
    : Wonka.Types.sourceT(ClientTypes.clientResponse('response)) => {
  let req =
    UrqlRequest.createRequest(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );
  let parse = request##parse;
  let optsJs = partialOpCtxToPartialOpCtxJs(opts);

  executeSubscriptionJs(~client, ~subscription=req, ~opts=?optsJs, ())
  |> Wonka.map((. result) => urqlClientResponseToReason(~parse, ~result));
};

/* Additional methods exposed on the Client. These are currently here mostly for
   posterity. You'd use them infrequently, if at all, in a typical workflow. */
[@bs.send]
external executeRequestOperation:
  (~client: t, ~operation: ClientTypes.operation) =>
  Wonka.Types.sourceT(ClientTypes.operationResult) =
  "executeRequestOperation";

[@bs.send]
external reexecuteOperation:
  (~client: t, ~operation: ClientTypes.operation) => unit =
  "reexecuteOperation";

[@bs.send]
external createRequestOperationJs:
  (
    ~client: t,
    ~operationType: string,
    ~request: UrqlTypes.graphqlRequest,
    ~opts: ClientTypes.partialOperationContextJs=?,
    unit
  ) =>
  ClientTypes.operation =
  "createRequestOperation";

let createRequestOperation = (~client, ~operationType, ~request, ~opts=?, ()) => {
  let op = ClientTypes.operationTypeToJs(operationType);
  let optsJs = partialOpCtxToPartialOpCtxJs(opts);
  createRequestOperationJs(
    ~client,
    ~operationType=op,
    ~request,
    ~opts=?optsJs,
    (),
  );
};

[@bs.send]
external dispatchOperation:
  (~client: t, ~operation: ClientTypes.operation) => unit =
  "dispatchOperation";

[@bs.send]
external queryJs:
  (
    ~client: t,
    ~query: string,
    ~variables: Js.Json.t=?,
    ~context: ClientTypes.partialOperationContextJs=?,
    unit
  ) =>
  Wonka.Types.sourceT('response) =
  "query";

let query =
    (
      ~client: t,
      ~request: UrqlTypes.request('response),
      ~opts: option(ClientTypes.partialOperationContext)=?,
      (),
    ) => {
  executeQuery(~client, ~request, ~opts?, ())
  |> Wonka.take(1)
  |> Wonka.toPromise;
};

let readQuery =
    (
      ~client: t,
      ~request: UrqlTypes.request('response),
      ~opts: option(ClientTypes.partialOperationContext)=?,
      (),
    ) => {
  let result = ref(None);
  executeQuery(~client, ~request, ~opts?, ())
  |> Wonka.subscribe((. data) => {result := Some(data)})
  |> (subscription => subscription.unsubscribe());
  result^;
};

[@bs.send]
external mutationJs:
  (
    ~client: t,
    ~query: string,
    ~variables: Js.Json.t=?,
    ~context: ClientTypes.partialOperationContextJs=?,
    unit
  ) =>
  Wonka.Types.sourceT('response) =
  "mutation";

let mutation =
    (
      ~client: t,
      ~request: UrqlTypes.request('response),
      ~opts: option(ClientTypes.partialOperationContext)=?,
      (),
    ) => {
  executeMutation(~client, ~request, ~opts?, ())
  |> Wonka.take(1)
  |> Wonka.toPromise;
};
