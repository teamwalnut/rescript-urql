module UrqlTypes = UrqlTypes;

module Client = {
  type clientResponse('response) =
    UrqlClient.ClientTypes.clientResponse('response) = {
      data: option('response),
      error: option(UrqlCombinedError.t),
      response: UrqlClient.ClientTypes.response('response),
    };

  include UrqlClient;
};

module Context = UrqlContext;
module Provider = UrqlContext.Provider;
module Consumer = UrqlContext.Consumer;

module Query = UrqlQuery;

module Mutation = UrqlMutation;

module Subscription = UrqlSubscription.Subscription;

module SubscriptionWithHandler = UrqlSubscription.SubscriptionWithHandler;

module Request = UrqlRequest;

module CombinedError = UrqlCombinedError;

module Exchanges = UrqlClient.UrqlExchanges;

module Hooks = {
  type hookResponse('ret, 'extensions) =
    UrqlTypes.hookResponse('ret, 'extensions) = {
      fetching: bool,
      data: option('ret),
      error: option(UrqlCombinedError.t),
      response: UrqlTypes.response('ret),
      extensions: option('extensions),
    };
  include UrqlUseMutation;
  include UrqlUseQuery;
  include UrqlUseSubscription;
};
