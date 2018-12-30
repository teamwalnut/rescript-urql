open Jest;
open ReasonUrql;
open TestUtils;

describe("UrqlMutation", () => {
  describe("UrqlMutation without variables", () => {
    /* Define the urqlMutation. */
    let mutation = Mutation.mutation(TestMutation.make());

    /* Setup the expected mutation string and urqlMutation object. */
    let expectedMutationString = "mutation likeAllDogs  {\nlikeAllDogs  {\nkey  \nlikes  \n}\n\n}\n";
    let expectedMutation =
      Mutation.urqlMutation(
        ~query=expectedMutationString,
        ~variables=Js.Json.object_(Js.Dict.empty()),
        (),
      );

    test("should return a valid urql mutation string", () =>
      Expect.(
        expect(mutation->Mutation.queryGet)
        |> toEqual(expectedMutationString)
      )
    );

    test(
      "should pass an empty JS object to the urql mutation object if no variables are provided",
      () =>
      Expect.(
        expect(mutation->Mutation.variablesGet)
        |> toEqual(Some(Js.Json.object_(Js.Dict.empty())))
      )
    );

    test("should return a valid urql mutation mutation", () =>
      Expect.(expect(mutation) |> toEqual(expectedMutation))
    );
  });

  describe("UrqlMutation with variables", () => {
    /* Define the urqlMutation with variables. */
    let mutation =
      Mutation.mutation(TestMutationWithVariable.make(~key="12345", ()));

    /* Setup the expected query string, variables, and urqlMutation object. */
    let expectedMutationString = "mutation likeDog($key: ID!)  {\nlikeDog(key: $key)  {\nname  \nkey  \nbreed  \nlikes  \n}\n\n}\n";
    let expectedVariables = Js.Dict.empty();
    Js.Dict.set(expectedVariables, "key", Js.Json.string("12345"));
    let expectedMutation =
      Mutation.urqlMutation(
        ~query=expectedMutationString,
        ~variables=Js.Json.object_(expectedVariables),
        (),
      );

    test("should return a valid urql mutation string", () =>
      Expect.(
        expect(mutation->Mutation.queryGet)
        |> toEqual(expectedMutationString)
      )
    );

    test("should pass the supplied variables to the urql mutation object", () =>
      Expect.(
        expect(mutation->Mutation.variablesGet)
        |> toEqual(Some(Js.Json.object_(expectedVariables)))
      )
    );

    test("should return a valid urql mutation object", () =>
      Expect.(expect(mutation) |> toEqual(expectedMutation))
    );
  });

  describe("Make functor", () => {
    describe("UrqlMutation without variables", () => {
      /* Define the urqlMutation. */
      module LikeAllDogs = Mutation.Make(TestMutation);

      /* Setup the expected query string and urqlMutation object. */
      let expectedMutationString = "mutation likeAllDogs  {\nlikeAllDogs  {\nkey  \nlikes  \n}\n\n}\n";
      let expectedMutation =
        Mutation.urqlMutation(
          ~query=expectedMutationString,
          ~variables=Js.Json.object_(Js.Dict.empty()),
          (),
        );

      test("should return a valid urql mutation object", () =>
        Expect.(expect(LikeAllDogs.mutation) |> toEqual(expectedMutation))
      );

      test(
        "should return an urql mutation object when mutationFn is invoked with no variables",
        () =>
        Expect.(
          expect(LikeAllDogs.mutationFn()) |> toEqual(expectedMutation)
        )
      );
    });

    describe("UrqlMutation with variables", () => {
      /* Define the urqlMutation. */
      module LikeDog = Mutation.Make(TestMutationWithVariable);

      /* Setup the expected mutation string, variables, and urqlMutation object. */
      let expectedMutationString = "mutation likeDog($key: ID!)  {\nlikeDog(key: $key)  {\nname  \nkey  \nbreed  \nlikes  \n}\n\n}\n";
      let expectedVariables = Js.Dict.empty();
      Js.Dict.set(expectedVariables, "key", Js.Json.string("12345"));
      let expectedMutation =
        Mutation.urqlMutation(
          ~query=expectedMutationString,
          ~variables=Js.Json.object_(expectedVariables),
          (),
        );

      test("should return a valid urql muation string", () =>
        Expect.(
          expect(LikeDog.mutation->Mutation.queryGet)
          |> toEqual(expectedMutationString)
        )
      );

      test(
        "should pass an empty JS object for variables if accessing the mutation object",
        () =>
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
            LikeDog.mutationFn(
              ~variables=Js.Json.object_(expectedVariables),
              (),
            )
            ->Mutation.variablesGet,
          )
          |> toEqual(Some(Js.Json.object_(expectedVariables)))
        )
      );

      test("mutationFn should return a valid urql mutation object", () =>
        Expect.(
          expect(
            LikeDog.mutationFn(
              ~variables=Js.Json.object_(expectedVariables),
              (),
            ),
          )
          |> toEqual(expectedMutation)
        )
      );
    });
  });
});