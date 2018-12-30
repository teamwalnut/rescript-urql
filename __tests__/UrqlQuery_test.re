open Jest;
open ReasonUrql;
open TestUtils;

describe("UrqlQuery", () => {
  describe("UrqlQuery without variables", () => {
    /* Define the urqlQuery. */
    let query = Query.query(TestQuery.make());

    /* Setup the expected query string and urqlQuery object. */
    let expectedQueryString = "query dogs  {\ndogs  {\nname  \nbreed  \ndescription  \n}\n\n}\n";
    let expectedQuery =
      Query.urqlQuery(
        ~query=expectedQueryString,
        ~variables=Js.Json.object_(Js.Dict.empty()),
        (),
      );

    test("should return a valid urql query string", () =>
      Expect.(expect(query->Query.queryGet) |> toEqual(expectedQueryString))
    );

    test(
      "should pass an empty JS object to the urql query object if no variables are provided",
      () =>
      Expect.(
        expect(query->Query.variablesGet)
        |> toEqual(Some(Js.Json.object_(Js.Dict.empty())))
      )
    );

    test("should return a valid urql query object", () =>
      Expect.(expect(query) |> toEqual(expectedQuery))
    );
  });

  describe("UrqlQuery with variables", () => {
    /* Define the urqlQuery with variables. */
    let query = Query.query(TestQueryWithVariable.make(~key="12345", ()));

    /* Setup the expected query string, variables, and urqlQuery object. */
    let expectedQueryString = "query dog($key: ID!)  {\ndog(key: $key)  {\nname  \nbreed  \ndescription  \n}\n\n}\n";
    let expectedVariables = Js.Dict.empty();
    Js.Dict.set(expectedVariables, "key", Js.Json.string("12345"));
    let expectedQuery =
      Query.urqlQuery(
        ~query=expectedQueryString,
        ~variables=Js.Json.object_(expectedVariables),
        (),
      );

    test("should return a valid urql query string", () =>
      Expect.(expect(query->Query.queryGet) |> toEqual(expectedQueryString))
    );

    test("should pass the supplied variables to the urql query object", () =>
      Expect.(
        expect(query->Query.variablesGet)
        |> toEqual(Some(Js.Json.object_(expectedVariables)))
      )
    );

    test("should return a valid urql query object", () =>
      Expect.(expect(query) |> toEqual(expectedQuery))
    );
  });

  describe("Make functor", () => {
    describe("UrqlQuery without variables", () => {
      /* Define the urqlQuery. */
      module GetDogs = Query.Make(TestQuery);

      /* Setup the expected query string and urqlQuery object. */
      let expectedQueryString = "query dogs  {\ndogs  {\nname  \nbreed  \ndescription  \n}\n\n}\n";
      let expectedQuery =
        Query.urqlQuery(
          ~query=expectedQueryString,
          ~variables=Js.Json.object_(Js.Dict.empty()),
          (),
        );

      test("should return a valid urql query object", () =>
        Expect.(expect(GetDogs.query) |> toEqual(expectedQuery))
      );

      test(
        "should return an urql query object when queryFn is invoked with no variables",
        () =>
        Expect.(expect(GetDogs.queryFn()) |> toEqual(expectedQuery))
      );
    });

    describe("UrqlQuery with variables", () => {
      /* Define the urqlQuery. */
      module GetDog = Query.Make(TestQueryWithVariable);

      /* Setup the expected query string and urqlQuery object. */
      let expectedQueryString = "query dog($key: ID!)  {\ndog(key: $key)  {\nname  \nbreed  \ndescription  \n}\n\n}\n";
      let expectedVariables = Js.Dict.empty();
      Js.Dict.set(expectedVariables, "key", Js.Json.string("12345"));
      let expectedQuery =
        Query.urqlQuery(
          ~query=expectedQueryString,
          ~variables=Js.Json.object_(expectedVariables),
          (),
        );

      test("should return a valid urql query string", () =>
        Expect.(
          expect(GetDog.query->Query.queryGet)
          |> toEqual(expectedQueryString)
        )
      );

      test(
        "should pass an empty JS object for variables if accessing the query object",
        () =>
        Expect.(
          expect(GetDog.query->Query.variablesGet)
          |> toEqual(Some(Js.Json.object_(Js.Dict.empty())))
        )
      );

      test(
        "should return queryFn – a function that can be invoked with variables",
        () =>
        Expect.(
          expect(
            GetDog.queryFn(~variables=Js.Json.object_(expectedVariables), ())
            ->Query.variablesGet,
          )
          |> toEqual(Some(Js.Json.object_(expectedVariables)))
        )
      );

      test("queryFn should return a valid urql query object", () =>
        Expect.(
          expect(
            GetDog.queryFn(
              ~variables=Js.Json.object_(expectedVariables),
              (),
            ),
          )
          |> toEqual(expectedQuery)
        )
      );
    });
  });
});