module Client = UrqlClient;

module Context = UrqlContext;
module Provider = UrqlContext.Provider;
module Consumer = UrqlContext.Consumer;

module Query = UrqlQuery;

module Mutation = UrqlMutation;

module Subscription = UrqlSubscription;

module Types = UrqlTypes;

module Request = UrqlRequest;

module Error = UrqlCombinedError;

module Exchanges = UrqlClient.UrqlExchanges;

module Hooks = {
  include UrqlUseMutation;
  include UrqlUseQuery;
  include UrqlUseSubscription;
};
