module UrqlTypes = UrqlTypes;

module Client = UrqlClient;

module Context = UrqlContext;
module Provider = UrqlContext.Provider;
module Consumer = UrqlContext.Consumer;

module Request = UrqlRequest;

module CombinedError = UrqlCombinedError;

module Exchanges = UrqlClient.Exchanges;

module Operations = UrqlOperations;

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
  include UrqlUseClient;
};
