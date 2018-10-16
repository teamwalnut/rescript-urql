open Jest;
open ReasonUrql;

describe("UrqlMutation", () => {
  describe("UrqlMutation with only query specified", () => {
    module LikeAllDogs = [%graphql
      {|
      mutation likeAllDogs {
        likeAllDogs {
          key
          likes
        }
      }
    |}
    ];

    let myMutation = Mutation.mutation(LikeAllDogs.make());
    let expectedMutation = "mutation likeAllDogs  {\nlikeAllDogs  {\nkey  \nlikes  \n}\n\n}\n";

    test("should return a valid urql mutation string", () =>
      Expect.(
        expect(myMutation->Mutation.queryGet) |> toEqual(expectedMutation)
      )
    );

    test("should return an empty JS object if no variables provided", () =>
      Expect.(
        expect(myMutation->Mutation.variablesGet)
        |> toEqual(Some(Js.Json.object_(Js.Dict.empty())))
      )
    );
  });

  describe("UrqlMutation with variables", () => {
    module LikeDog = [%graphql
      {|
        mutation likeDog($key: ID!) {
          likeDog(key: $key) {
            key
            likes
          }
        }
      |}
    ];

    let myMutation = Mutation.mutation(LikeDog.make(~key="12345", ()));
    let expectedMutation = "mutation likeDog($key: ID!)  {\nlikeDog(key: $key)  {\nkey  \nlikes  \n}\n\n}\n";
    let variables = Js.Dict.empty();
    Js.Dict.set(variables, "key", Js.Json.string("12345"));

    test("should return a valid urql mutation string", () =>
      Expect.(
        expect(myMutation->Mutation.queryGet) |> toEqual(expectedMutation)
      )
    );

    test("should return the variables passed to the mutation", () =>
      Expect.(
        expect(myMutation->Mutation.variablesGet)
        |> toEqual(Some(Js.Json.object_(variables)))
      )
    );
  });
});