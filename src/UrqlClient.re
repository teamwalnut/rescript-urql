/* Model fetchOptions as a variant, and unwrap the user supplied prop to an object
   to pass directly to urql. */
type fetchOptions =
  | FetchObj(Fetch.requestInit)
  | FetchFn(unit => Fetch.requestInit);

let unwrapFetchOptions: fetchOptions => Fetch.requestInit =
  (opts: fetchOptions) =>
    switch (opts) {
    | FetchObj(obj) => obj
    | FetchFn(fn) => fn()
    };

type data;
type write = (~hash: string, ~data: data) => Js.Promise.t(unit);
type read('value) = (~hash: string) => Js.Promise.t('value);
type invalidate = (~hash: string) => Js.Promise.t(unit);
type invalidateAll = unit => Js.Promise.t(unit);
type update('store, 'value) =
  (~callback: (~store: 'store, ~key: string, ~value: 'value) => unit) =>
  Js.Promise.t(unit);

[@bs.deriving jsConverter]
type cache('store, 'value) = {
  write,
  read: read('value),
  invalidate,
  invalidateAll,
  update: update('store, 'value),
};

type cacheJs('store, 'value) = {
  .
  "write": write,
  "read": read('value),
  "invalidate": invalidate,
  "invalidateAll": invalidateAll,
  "update": update('store, 'value),
};

[@bs.deriving abstract]
type clientConfig('store, 'value) = {
  url: string,
  [@bs.optional]
  cache: cacheJs('store, 'value),
  [@bs.optional]
  initialCache: 'store,
  [@bs.optional]
  fetchOptions: Fetch.requestInit,
};

type client;

[@bs.new] [@bs.module "urql"]
external createClient: clientConfig('store, 'value) => client = "Client";

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
    (
      ~url: string,
      ~cache=?,
      ~initialCache=?,
      ~fetchOptions=FetchObj(Fetch.RequestInit.make()),
      (),
    ) => {
  /* Generate the client config */
  let config =
    switch (cache) {
    | Some(c) =>
      clientConfig(
        ~url,
        ~cache=c |> cacheToJs,
        ~initialCache?,
        ~fetchOptions=unwrapFetchOptions(fetchOptions),
        (),
      )
    | None =>
      clientConfig(
        ~url,
        ~initialCache?,
        ~fetchOptions=unwrapFetchOptions(fetchOptions),
        (),
      )
    };

  createClient(config);
};