type t = UrqlClientTypes.t;

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

type clientOptions('fetchOptions, 'fetchImpl) = {
  url: string,
  fetchOptions: option('fetchOptions),
  exchanges: array(UrqlExchanges.t),
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
      ~exchanges=[|
                   UrqlExchanges.dedupExchange,
                   UrqlExchanges.cacheExchange,
                   UrqlExchanges.fetchExchange,
                 |],
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
    requestPolicy: requestPolicy->UrqlTypes.requestPolicyToJs,
  };

  client(options);
};

/* Execution methods on the client. These allow you to imperatively execute GraphQL
   operations outside of components or hooks. */
[@bs.send]
external executeQueryJs:
  (
    ~client: t,
    ~query: UrqlTypes.graphqlRequest,
    ~opts: UrqlClientTypes.PartialOperationContextJs.t=?,
    unit
  ) =>
  Wonka.Types.sourceT(UrqlClientTypes.operationResult) =
  "executeQuery";

let executeQuery = (~client, ~request, ~opts=?, ()) => {
  let req =
    UrqlRequest.createRequest(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );
  let parse = request##parse;
  let optsJs = UrqlClientTypes.decodePartialOperationContext(opts);

  executeQueryJs(~client, ~query=req, ~opts=?optsJs, ())
  |> Wonka.map((. response) =>
       UrqlResponse.urqlClientResponseToReason(~response, ~parse)
     );
};

[@bs.send]
external executeMutationJs:
  (
    ~client: t,
    ~mutation: UrqlTypes.graphqlRequest,
    ~opts: UrqlClientTypes.PartialOperationContextJs.t=?,
    unit
  ) =>
  Wonka.Types.sourceT(UrqlClientTypes.operationResult) =
  "executeMutation";

let executeMutation = (~client, ~request, ~opts=?, ()) => {
  let req =
    UrqlRequest.createRequest(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );
  let parse = request##parse;
  let optsJs = UrqlClientTypes.decodePartialOperationContext(opts);

  executeMutationJs(~client, ~mutation=req, ~opts=?optsJs, ())
  |> Wonka.map((. response) =>
       UrqlResponse.urqlClientResponseToReason(~response, ~parse)
     );
};

[@bs.send]
external executeSubscriptionJs:
  (
    ~client: t,
    ~subscription: UrqlTypes.graphqlRequest,
    ~opts: UrqlClientTypes.PartialOperationContextJs.t=?,
    unit
  ) =>
  Wonka.Types.sourceT(UrqlClientTypes.operationResult) =
  "executeSubscription";

let executeSubscription = (~client, ~request, ~opts=?, ()) => {
  let req =
    UrqlRequest.createRequest(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );
  let parse = request##parse;
  let optsJs = UrqlClientTypes.decodePartialOperationContext(opts);

  executeSubscriptionJs(~client, ~subscription=req, ~opts=?optsJs, ())
  |> Wonka.map((. response) =>
       UrqlResponse.urqlClientResponseToReason(~response, ~parse)
     );
};

let query = (~client, ~request, ~opts=?, ()) => {
  executeQuery(~client, ~request, ~opts?, ())
  |> Wonka.take(1)
  |> Wonka.toPromise;
};

let mutation = (~client, ~request, ~opts=?, ()) => {
  executeMutation(~client, ~request, ~opts?, ())
  |> Wonka.take(1)
  |> Wonka.toPromise;
};
