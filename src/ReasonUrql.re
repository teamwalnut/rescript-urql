module Client = UrqlClient;

module Context = UrqlContext;
module Provider = UrqlContext.Provider;
module Consumer = UrqlContext.Consumer;

module Query = UrqlQuery;

module Mutation = UrqlMutation;

module Subscription = UrqlSubscription.Subscription;

module SubscriptionWithHandler = UrqlSubscription.SubscriptionWithHandler;

module Types = UrqlTypes;

module Request = UrqlRequest;

module Error = UrqlCombinedError;

module Exchanges = UrqlClient.UrqlExchanges;

module Hooks = {
  type hookResponse('ret) =
    Types.hookResponse('ret) = {
      fetching: bool,
      data: option('ret),
      error: option(UrqlCombinedError.t),
      response: Types.response('ret),
    };
  include UrqlUseMutation;
  include UrqlUseQuery;
  include UrqlUseSubscription;
};
