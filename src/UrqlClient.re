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
  (urqlClient, UrqlQuery.urqlQuery, bool) => Js.Promise.t('a) =
  "";

[@bs.send]
external executeMutation :
  (urqlClient, UrqlMutation.urqlMutation) => Js.Promise.t('a) =
  "";