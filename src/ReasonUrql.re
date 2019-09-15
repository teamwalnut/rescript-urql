module UrqlTypes = UrqlTypes;

module Client = {
  type clientResponse('response) =
    UrqlClient.Types.clientResponse('response) = {
      data: option('response),
      error: option(UrqlCombinedError.combinedError),
      response: UrqlClient.Types.response('response),
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

module Error = UrqlCombinedError;

module Exchanges = UrqlClient.UrqlExchanges;

module Hooks = {
  type hookResponse('ret) =
    UrqlTypes.hookResponse('ret) = {
      fetching: bool,
      data: option('ret),
      error: option(UrqlCombinedError.combinedError),
      response: UrqlTypes.response('ret),
    };
  include UrqlUseMutation;
  include UrqlUseQuery;
  include UrqlUseSubscription;
};
