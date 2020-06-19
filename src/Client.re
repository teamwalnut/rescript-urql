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

  type subscriptionExchangeOpts = {
    forwardSubscription: subscriptionForwarder,
  };

  [@bs.module "urql"]
  external subscriptionExchange: subscriptionExchangeOpts => t =
    "subscriptionExchange";

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
};

[@bs.new] [@bs.module "urql"]
external client: clientOptions('fetchOptions, 'fetchImpl) => t = "Client";

/* `make` is equivalent to urql's `createClient`.
   We opt to use `make` here to adhere to standards in the Reason community. */
let make =
    (
      ~url,
      ~fetchOptions=?,
      ~requestPolicy=`CacheFirst,
      ~exchanges=Exchanges.defaultExchanges,
      ~suspense=false,
      ~fetch=?,
      (),
    ) => {
  let options = {
    url,
    fetchOptions: fetchOptions->unwrapFetchOptions,
    exchanges,
    suspense,
    fetch: fetch->unwrapFetchImpl,
    requestPolicy: requestPolicy->Types.requestPolicyToJs,
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
  response: response('response),
};

/**
 * A function to convert the JS response from a client.execute*
 * methods to typed a Reason record.
 */
let urqlClientResponseToReason = (~response: Types.operationResult, ~parse) => {
  let data = response.data->Belt.Option.map(parse);
  let error =
    response.error->Belt.Option.map(CombinedError.combinedErrorToRecord);

  let response =
    switch (data, error) {
    | (Some(data), _) => Data(data)
    | (None, Some(error)) => Error(error)
    | (None, None) => Empty
    };

  {data, error, response};
};

[@bs.val] [@bs.module "urql"]
external createRequest:
  (~query: string, ~variables: Js.Json.t=?, unit) => Types.graphqlRequest =
  "createRequest";

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
  Wonka.Types.sourceT(Types.operationResult) =
  "executeQuery";

let executeQuery =
    (
      ~client,
      ~request,
      ~fetchOptions=?,
      ~requestPolicy=?,
      ~url=?,
      ~pollInterval=?,
      ~meta=?,
      (),
    ) => {
  let req =
    createRequest(~query=request##query, ~variables=request##variables, ());
  let parse = request##parse;
  let optsJs =
    Types.partialOperationContext(
      ~fetchOptions?,
      ~requestPolicy=?Belt.Option.map(requestPolicy, Types.requestPolicyToJs),
      ~url?,
      ~pollInterval?,
      ~meta?,
      (),
    );

  executeQueryJs(~client, ~query=req, ~opts=optsJs, ())
  |> Wonka.map((. response) => urqlClientResponseToReason(~response, ~parse));
};

[@bs.send]
external executeMutationJs:
  (
    ~client: t,
    ~mutation: Types.graphqlRequest,
    ~opts: Types.partialOperationContext=?,
    unit
  ) =>
  Wonka.Types.sourceT(Types.operationResult) =
  "executeMutation";

let executeMutation =
    (
      ~client,
      ~request,
      ~fetchOptions=?,
      ~requestPolicy=?,
      ~url=?,
      ~pollInterval=?,
      ~meta=?,
      (),
    ) => {
  let req =
    createRequest(~query=request##query, ~variables=request##variables, ());
  let parse = request##parse;
  let optsJs =
    Types.partialOperationContext(
      ~fetchOptions?,
      ~requestPolicy=?Belt.Option.map(requestPolicy, Types.requestPolicyToJs),
      ~url?,
      ~pollInterval?,
      ~meta?,
      (),
    );

  executeMutationJs(~client, ~mutation=req, ~opts=optsJs, ())
  |> Wonka.map((. response) => urqlClientResponseToReason(~response, ~parse));
};

[@bs.send]
external executeSubscriptionJs:
  (
    ~client: t,
    ~subscription: Types.graphqlRequest,
    ~opts: Types.partialOperationContext=?,
    unit
  ) =>
  Wonka.Types.sourceT(Types.operationResult) =
  "executeSubscription";

let executeSubscription =
    (
      ~client,
      ~request,
      ~fetchOptions=?,
      ~requestPolicy=?,
      ~url=?,
      ~pollInterval=?,
      ~meta=?,
      (),
    ) => {
  let req =
    createRequest(~query=request##query, ~variables=request##variables, ());
  let parse = request##parse;
  let optsJs =
    Types.partialOperationContext(
      ~fetchOptions?,
      ~requestPolicy=?Belt.Option.map(requestPolicy, Types.requestPolicyToJs),
      ~url?,
      ~pollInterval?,
      ~meta?,
      (),
    );

  executeSubscriptionJs(~client, ~subscription=req, ~opts=optsJs, ())
  |> Wonka.map((. response) => urqlClientResponseToReason(~response, ~parse));
};

let query =
    (
      ~client,
      ~request,
      ~fetchOptions=?,
      ~requestPolicy=?,
      ~url=?,
      ~pollInterval=?,
      ~meta=?,
      (),
    ) => {
  executeQuery(
    ~client,
    ~request,
    ~fetchOptions?,
    ~requestPolicy?,
    ~url?,
    ~pollInterval?,
    ~meta?,
    (),
  )
  |> Wonka.take(1)
  |> Wonka.toPromise;
};

let mutation =
    (
      ~client,
      ~request,
      ~fetchOptions=?,
      ~requestPolicy=?,
      ~url=?,
      ~pollInterval=?,
      ~meta=?,
      (),
    ) => {
  executeMutation(
    ~client,
    ~request,
    ~fetchOptions?,
    ~requestPolicy?,
    ~url?,
    ~pollInterval?,
    ~meta?,
    (),
  )
  |> Wonka.take(1)
  |> Wonka.toPromise;
};
