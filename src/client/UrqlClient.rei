type t = UrqlClientTypes.t;

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

let make:
  (
    ~url: string,
    ~fetchOptions: fetchOptions('a)=?,
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~exchanges: array(UrqlExchanges.t)=?,
    ~suspense: bool=?,
    ~fetch: fetchImpl('a)=?,
    unit
  ) =>
  t;

let executeQuery:
  (
    ~client: t,
    ~request: UrqlTypes.request('response),
    ~opts: UrqlClientTypes.partialOperationContext=?,
    unit
  ) =>
  Wonka.Types.sourceT(UrqlClientTypes.clientResponse('response));

let executeMutation:
  (
    ~client: t,
    ~request: UrqlTypes.request('response),
    ~opts: UrqlClientTypes.partialOperationContext=?,
    unit
  ) =>
  Wonka.Types.sourceT(UrqlClientTypes.clientResponse('response));

let executeSubscription:
  (
    ~client: t,
    ~request: UrqlTypes.request('response),
    ~opts: UrqlClientTypes.partialOperationContext=?,
    unit
  ) =>
  Wonka.Types.sourceT(UrqlClientTypes.clientResponse('response));

let query:
  (
    ~client: t,
    ~request: UrqlTypes.request('response),
    ~opts: UrqlClientTypes.partialOperationContext=?,
    unit
  ) =>
  Js.Promise.t(UrqlClientTypes.clientResponse('response));

let mutation:
  (
    ~client: t,
    ~request: UrqlTypes.request('response),
    ~opts: UrqlClientTypes.partialOperationContext=?,
    unit
  ) =>
  Js.Promise.t(UrqlClientTypes.clientResponse('response));
