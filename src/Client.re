[@bs.deriving abstract]
type urqlClientConfig('fetchOptions) = {
  url: string,
  [@bs.optional]
  fetchOptions: [
    | `FetchObj('fetchOptions)
    | `FetchFunc(unit => 'fetchOptions)
  ],
};

type urqlClient;

[@bs.new] [@bs.module "urql"]
external client : urqlClientConfig('fetchOptions) => urqlClient = "Client";

[@bs.send]
external executeQuery :
  (urqlClient, Query.urqlQuery, bool) => Js.Promise.t('a) =
  "";

[@bs.send]
external executeMutation :
  (urqlClient, Mutation.urqlMutation) => Js.Promise.t('a) =
  "";
