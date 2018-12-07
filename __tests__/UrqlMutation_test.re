open Jest;
open ReasonUrql;

describe("UrqlMutation", () => {
  describe("UrqlMutation with only query specified", () => {
    let myMutation = Mutation.mutation(TestUtils.TestMutation.make());
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
    let myMutation =
      Mutation.mutation(
        TestUtils.TestMutationWithVariable.make(~key="12345", ()),
      );
    let expectedMutation = "mutation likeDog($key: ID!)  {\nlikeDog(key: $key)  {\nname  \nkey  \nbreed  \nlikes  \n}\n\n}\n";
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

  describe("Make functor", () => {
    module LikeDog = Mutation.Make(TestUtils.TestMutationWithVariable);
    let expectedQuery = "mutation likeDog($key: ID!)  {\nlikeDog(key: $key)  {\nname  \nkey  \nbreed  \nlikes  \n}\n\n}\n";
    let variables = Js.Dict.empty();
    Js.Dict.set(variables, "key", Js.Json.string("12345"));

    test("should return a valid urql query string", () =>
      Expect.(
        expect(LikeDog.mutation->Mutation.queryGet) |> toEqual(expectedQuery)
      )
    );

    test("should return an empty JS object", () =>
      Expect.(
        expect(LikeDog.mutation->Mutation.variablesGet)
        |> toEqual(Some(Js.Json.object_(Js.Dict.empty())))
      )
    );

    test(
      "should return mutationFn – a function that can be invoked with variables",
      () =>
      Expect.(
        expect(
          LikeDog.mutationFn(~variables=Js.Json.object_(variables), ())
          ->Mutation.variablesGet,
        )
        |> toEqual(Some(Js.Json.object_(variables)))
      )
    );
  });
});