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
type read('queryData) = (~hash: string) => Js.Promise.t('queryData);
type invalidate = (~hash: string) => Js.Promise.t(unit);
type invalidateAll = unit => Js.Promise.t(unit);
type update('queryData, 'store) =
  (~callback: ('store, string, 'queryData) => unit) => Js.Promise.t(unit);

[@bs.deriving jsConverter]
type cache('queryData, 'store) = {
  write,
  read: read('queryData),
  invalidate,
  invalidateAll,
  update: update('queryData, 'store),
};

type cacheJs('queryData, 'store) = {
  .
  "write": write,
  "read": read('queryData),
  "invalidate": invalidate,
  "invalidateAll": invalidateAll,
  "update": update('store, 'queryData),
};

[@bs.deriving abstract]
type clientConfig('initialCache, 'queryData, 'store) = {
  url: string,
  [@bs.optional]
  cache: cacheJs('queryData, 'store),
  [@bs.optional]
  initialCache: 'initialCache,
  [@bs.optional]
  fetchOptions: Fetch.requestInit,
};

type client;

[@bs.new] [@bs.module "urql"]
external createClient:
  clientConfig('queryData, 'initialCache, 'store) => client =
  "Client";

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
      ~initialCache: 'initialCache=?,
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