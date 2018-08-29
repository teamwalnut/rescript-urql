type fetchOptions =
  | FetchObj(Fetch.requestInit)
  | FetchFn(unit => Fetch.requestInit);

/* `urql` does this typecheck as well, but it's more elegant on the Reason end.
   We'll do it here and always pass an object (Js.t). */
let unwrapFetchOptions: fetchOptions => Fetch.requestInit =
  (opts: fetchOptions) =>
    switch (opts) {
    | FetchObj(obj) => obj
    | FetchFn(fn) => fn()
    };

[@bs.deriving abstract]
type clientConfig = {
  url: string,
  [@bs.optional]
  fetchOptions: Fetch.requestInit,
};

type client;

[@bs.new] [@bs.module "urql"]
external createClient: clientConfig => client = "Client";

[@bs.send]
external executeQuery:
  (~client: client, ~query: UrqlQuery.urqlQuery, ~skipCache: bool) =>
  Js.Promise.t('a) =
  "";

[@bs.send]
external executeMutation:
  (~client: client, ~mutation: UrqlMutation.urqlMutation) => Js.Promise.t('a) =
  "";

let make =
    (~url: string, ~fetchOptions=FetchObj(Fetch.RequestInit.make()), ()) => {
  /* Generate the client config */
  let config =
    clientConfig(~url, ~fetchOptions=unwrapFetchOptions(fetchOptions), ());

  createClient(config);
};