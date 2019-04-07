type t;
type fetchOptions =
  | FetchObj(Fetch.requestInit)
  | FetchFn(unit => Fetch.requestInit);

let unwrapFetchOptions = fetchOptions =>
  switch (fetchOptions) {
  | FetchObj(obj) => obj
  | FetchFn(fn) => fn()
  };

module UrqlExchanges = {
  [@bs.deriving jsConverter]
  type operationType = [
    | [@bs.as "subscription"] `Subscription
    | [@bs.as "query"] `Query
    | [@bs.as "mutation"] `Mutation
    | [@bs.as "teardown"] `Teardown
  ];

  [@bs.deriving abstract]
  type operationContext = {
    [@bs.optional]
    fetchOptions: Fetch.requestInit,
    requestPolicy: UrqlTypes.requestPolicy,
    url: string,
  };

  [@bs.deriving abstract]
  type operation = {
    [@bs.optional]
    variables: Js.Json.t,
    key: int,
    query: string,
    operationName: operationType,
    context: operationContext,
  };

  type operationData;

  [@bs.deriving abstract]
  type operationResult = {
    operation,
    [@bs.optional]
    data: operationData,
    [@bs.optional]
    error: UrqlCombinedError.t,
  };

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
  [@bs.module "urql"] external subscriptionExchange: exchange = "";
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
  (~client: t, ~query: UrqlTypes.graphqlRequest) => Wonka.Types.sourceT('a) =
  "";

[@bs.send]
external executeMutation:
  (~client: t, ~mutation: UrqlTypes.graphqlRequest) => Wonka.Types.sourceT('a) =
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