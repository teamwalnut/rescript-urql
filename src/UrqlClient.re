open UrqlTypes;
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

/* A module for binding exchange types and urql's exposed exchanges. Since this module
   references UrqlClient and UrqlClient references this module,
   they need to be co-located to avoid a cyclic dependency error. */
module UrqlExchanges = {
  type exchangeIO =
    Wonka.Types.sourceT(operation) => Wonka.Types.sourceT(operationResult);

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
    context: operationContext,
  };

  type subscriptionForwarder =
    subscriptionOperation => observableLike(executionResult);

  [@bs.deriving abstract]
  type subscriptionExchangeOpts = {
    forwardSubscription: subscriptionForwarder,
  };

  [@bs.module "urql"]
  external subscriptionExchange: subscriptionExchangeOpts => exchange = "";
};

[@bs.deriving abstract]
type clientOptions('a) = {
  url: string,
  [@bs.optional]
  fetchOptions: 'a,
  [@bs.optional]
  exchanges: array(UrqlExchanges.exchange),
};

[@bs.new] [@bs.module "urql"]
external client: clientOptions('a) => t = "Client";

[@bs.send]
external urqlExecuteQuery:
  (
    ~client: t,
    ~query: graphqlRequest,
    ~opts: partialOperationContext=?,
    unit
  ) =>
  Wonka.Types.sourceT({. "data": Js.Json.t}) =
  "executeQuery";

let executeQuery =
    (
      ~client: t,
      ~request: request('response),
      ~opts: option(partialOperationContext)=?,
      (),
    )
    : Wonka.Types.sourceT('response) => {
  let req =
    UrqlRequest.createRequest(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );
  let parse = request##parse;

  urqlExecuteQuery(~client, ~query=req, ~opts?, ())
  |> Wonka.map((. res) => parse(res##data));
};

[@bs.send]
external urqlExecuteMutation:
  (
    ~client: t,
    ~mutation: graphqlRequest,
    ~opts: partialOperationContext=?,
    unit
  ) =>
  Wonka.Types.sourceT({. "data": Js.Json.t}) =
  "executeMutation";

let executeMutation =
    (
      ~client: t,
      ~request: request('response),
      ~opts: option(partialOperationContext)=?,
      (),
    )
    : Wonka.Types.sourceT('response) => {
  let req =
    UrqlRequest.createRequest(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );
  let parse = request##parse;

  urqlExecuteMutation(~client, ~mutation=req, ~opts?, ())
  |> Wonka.map((. res) => parse(res##data));
};

[@bs.send]
external urqlExecuteSubscription:
  (
    ~client: t,
    ~subscription: graphqlRequest,
    ~opts: partialOperationContext=?,
    unit
  ) =>
  Wonka.Types.sourceT({. "data": Js.Json.t}) =
  "executeSubscription";

let executeSubscription =
    (
      ~client: t,
      ~request: request('response),
      ~opts: option(partialOperationContext)=?,
      (),
    )
    : Wonka.Types.sourceT('response) => {
  let req =
    UrqlRequest.createRequest(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );
  let parse = request##parse;

  urqlExecuteSubscription(~client, ~subscription=req, ~opts?, ())
  |> Wonka.map((. res) => parse(res##data));
};

[@bs.send]
external executeRequestOperation:
  (~client: t, ~operation: operation) => Wonka.Types.sourceT(operationResult) =
  "";

[@bs.send]
external reexecuteOperation: (~client: t, ~operation: operation) => unit = "";

[@bs.send]
external createRequestOperationJs:
  (
    ~client: t,
    ~operationType: string,
    ~request: graphqlRequest,
    ~opts: partialOperationContext=?,
    unit
  ) =>
  operation =
  "createRequestOperation";

let createRequestOperation = (~client, ~operationType, ~request, ~opts=?, ()) => {
  let op = operationTypeToJs(operationType);
  createRequestOperationJs(~client, ~operationType=op, ~request, ~opts?, ());
};

[@bs.send]
external dispatchOperation: (~client: t, ~operation: operation) => unit = "";

/*
   `make` is equivalent to urql's `createClient`.
   We opt to use `make` here to adhere to standards in the Reason community.
 */
let make =
    (
      ~url,
      ~fetchOptions=?,
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
