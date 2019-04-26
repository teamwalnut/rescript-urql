type t;

/* Helpers for supporting polymorphic fetchOptions. */
type fetchOptions =
  | FetchObj(Fetch.requestInit)
  | FetchFn(unit => Fetch.requestInit);

let unwrapFetchOptions = fetchOptions =>
  switch (fetchOptions) {
  | FetchObj(obj) => obj
  | FetchFn(fn) => fn()
  };

/* A module for binding exchange types and urql's exposed exchanges. Since this module references UrqlClient and UrqlClient references
   this module, they need to be co-located to avoid a cyclic dependency error. */
module UrqlExchanges = {
  type exchangeIO =
    Wonka.Types.sourceT(UrqlTypes.operation) =>
    Wonka.Types.sourceT(UrqlTypes.operationResult);

  [@bs.deriving abstract]
  type exchangeInput = {
    forward: exchangeIO,
    client: t,
  };

  type exchange = exchangeInput => exchangeIO;

  [@bs.module "urql"] external cacheExchange: exchange = "";
  [@bs.module "urql"] external debugExchange: exchange = "";
  [@bs.module "urql"] external dedupExchange: exchange = "";
  [@bs.module "urql"] external fallbackExchangeIO: exchangeIO = "";
  [@bs.module "urql"] external fetchExchange: exchange = "";
  [@bs.module "urql"]
  external composeExchanges: array(exchange) => exchange = "";
  [@bs.module "urql"] external defaultExchanges: array(exchange) = "";

  /* Specific types for the subscriptionExchange. */
  [@bs.deriving abstract]
  type observerLike('a) = {
    next: 'a => unit,
    error: Js.Exn.t => unit,
    complete: unit => unit,
  };

  [@bs.deriving abstract]
  type observableLike('a) = {
    subscribe:
      observerLike('a) => {. [@bs.meth] "unsubscribe": unit => unit},
  };

  [@bs.deriving abstract]
  type subscriptionOperation = {
    query: string,
    [@bs.optional]
    variables: Js.Json.t,
    key: string,
    context: UrqlTypes.operationContext,
  };

  type subscriptionForwarder('a) =
    subscriptionOperation => observableLike(UrqlTypes.executionResult('a));

  [@bs.deriving abstract]
  type subscriptionExchangeOpts('a) = {
    forwardSubscription: subscriptionForwarder('a),
  };

  [@bs.module "urql"]
  external subscriptionExchange: subscriptionExchangeOpts('a) => exchange =
    "";
};

[@bs.deriving abstract]
type clientOptions = {
  url: string,
  [@bs.optional]
  fetchOptions: Fetch.requestInit,
  [@bs.optional]
  exchanges: array(UrqlExchanges.exchange),
};

[@bs.new] [@bs.module "urql"] external client: clientOptions => t = "Client";

[@bs.send]
external executeQuery:
  (
    ~client: t,
    ~query: UrqlTypes.graphqlRequest,
    ~opts: option(UrqlTypes.partialOperationContext)=?,
    unit
  ) =>
  Wonka.Types.sourceT('a) =
  "";

[@bs.send]
external executeMutation:
  (
    ~client: t,
    ~mutation: UrqlTypes.graphqlRequest,
    ~opts: UrqlTypes.partialOperationContext=?,
    unit
  ) =>
  Wonka.Types.sourceT('a) =
  "";

[@bs.send]
external executeSubscription:
  (
    ~client: t,
    ~subscription: UrqlTypes.graphqlRequest,
    ~opts: option(UrqlTypes.partialOperationContext)=?,
    unit
  ) =>
  Wonka.Types.sourceT('a) =
  "";
/*
   `make` is equivalent to urql's `createClient`.
   We opt to use `make` here to adhere to standards in the Reason community.
 */
let make =
    (
      ~url,
      ~fetchOptions=FetchObj(Fetch.RequestInit.make()),
      ~exchanges=[|
                   UrqlExchanges.dedupExchange,
                   UrqlExchanges.cacheExchange,
                   UrqlExchanges.fetchExchange,
                 |],
      (),
    ) => {
  let options =
    clientOptions(
      ~url,
      ~fetchOptions=unwrapFetchOptions(fetchOptions),
      ~exchanges,
      (),
    );
  client(options);
};