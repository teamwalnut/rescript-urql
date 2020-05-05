type exchangeIO =
  Wonka.Types.sourceT(UrqlClientTypes.operation) =>
  Wonka.Types.sourceT(UrqlClientTypes.operationResult);

type exchangeInput = {
  forward: exchangeIO,
  client: UrqlClientTypes.t,
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
  context: UrqlClientTypes.operationContext,
};

type subscriptionForwarder =
  subscriptionOperation => observableLike(UrqlClientTypes.executionResult);

type subscriptionExchangeOpts = {forwardSubscription: subscriptionForwarder};

[@bs.module "urql"]
external subscriptionExchange: subscriptionExchangeOpts => exchange =
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
external restoreData: (~exchange: exchange, ~restore: Js.Json.t) => Js.Json.t =
  "restoreData";
[@bs.send]
external extractData: (~exchange: exchange) => Js.Json.t = "extractData";

[@bs.module "urql"]
external ssrExchange:
  (~ssrExchangeParams: ssrExchangeParams=?, unit) => exchange =
  "ssrExchange";
