/* Sample Queries and Mutations for tests. */
module TestQuery = [%graphql
  {|
    query dogs {
      dogs {
        name
        breed
        description
      }
    }
  |}
];

module TestQueryWithVariable = [%graphql
  {|
    query dog($key: ID!) {
      dog(key: $key) {
        name
        breed
        description
      }
    }
  |}
];

module TestMutation = [%graphql
  {|
    mutation likeAllDogs {
      likeAllDogs {
        key
        likes
      }
    }
  |}
];

module TestMutationWithVariable = [%graphql
  {|
    mutation likeDog($key: ID!) {
      likeDog(key: $key) {
        name
        key
        breed
        likes
      }
    }
  |}
];

/* Simple, no-op cache for tests. */
/* Mock out cache functions with no-ops for brevity. */
let write = (~hash as _, ~data as _) =>
  Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a));

let read: UrqlClient.read(string) =
  (~hash as _) =>
    Js.Promise.make((~resolve, ~reject as _) => resolve(. "data"));

let invalidate: UrqlClient.invalidate =
  (~hash as _) =>
    Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a));

let invalidateAll: UrqlClient.invalidateAll =
  () => Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a));

let update: UrqlClient.update(Js.Dict.t(string), string) =
  (
    ~callback as
      _: (~store: Js.Dict.t(string), ~key: string, ~value: string) => unit,
  ) =>
    Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a));

let testCache: UrqlClient.cache(Js.Dict.t(string), string) = {
  write,
  read,
  invalidate,
  invalidateAll,
  update,
};

let testCacheJs: UrqlClient.cacheJs(Js.Dict.t(string), string) = {
  "write": write,
  "read": read,
  "invalidate": invalidate,
  "invalidateAll": invalidateAll,
  "update": update,
};

/* Simple, no-op cache functions passed to Connect. */
/* Mock out cache functions with no-ops for brevity. */
let readConnect = (~query as _) =>
  Js.Promise.make((~resolve, ~reject as _) => resolve(. "data"));
let invalidateConnect = (~query as _: option(UrqlQuery.urqlQuery)=?, ()) =>
  Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a));
let invalidateAllConnect = () =>
  Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a));
let updateConnect =
    (
      ~callback as
        _: (~store: Js.Dict.t(string), ~key: string, ~value: string) => unit,
    ) =>
  Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a));

let testCacheConnect: UrqlConnect.cache(Js.Dict.t(string), string) = {
  read: readConnect,
  invalidate: invalidateConnect,
  invalidateAll: invalidateAllConnect,
  update: updateConnect,
};

let testCacheConnectJs: UrqlConnect.cacheJs(Js.Dict.t(string), string) = {
  "read": readConnect,
  "invalidate": invalidateConnect,
  "invalidateAll": invalidateAllConnect,
  "update": updateConnect,
};

/* Sample data provided by urql's render prop. */
let urqlData = {
  "loaded": true,
  "data":
    Js.Nullable.return({
      "dogs": [|{"key": "VmeRTX7j-", "name": "Dixie", "breed": "Pit Bull"}|],
    }),
  "error": Js.Nullable.null,
  "cache": {
    "invalidateAll": () =>
      Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a)),
    "invalidate": (~query as _: option(UrqlQuery.urqlQuery)=?, ()) =>
      Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a)),
    "update":
      (
        ~callback as
          _:
            (~store: Js.Dict.t(string), ~key: string, ~value: string) => unit,
      ) =>
      Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a)),
    "read": (~query as _: UrqlQuery.urqlQuery) =>
      Js.Promise.make((~resolve, ~reject as _) => resolve(. "data")),
  },
};