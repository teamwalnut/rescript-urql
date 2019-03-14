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