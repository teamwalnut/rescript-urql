open Jest;
open ReasonUrql;

describe("UrqlQuery", () => {
  describe("UrqlQuery with only query specified", () => {
    let myQuery = Query.query(TestUtils.TestQuery.make());
    let expectedQuery = "query dogs  {\ndogs  {\nname  \nbreed  \ndescription  \n}\n\n}\n";

    test("should return a valid urql query string", () =>
      Expect.(expect(myQuery->Query.queryGet) |> toEqual(expectedQuery))
    );

    test("should return an empty JS object if no variables provided", () =>
      Expect.(
        expect(myQuery->Query.variablesGet)
        |> toEqual(Some(Js.Json.object_(Js.Dict.empty())))
      )
    );
  });

  describe("UrqlQuery with variables", () => {
    let myQuery =
      Query.query(TestUtils.TestQueryWithVariable.make(~key="12345", ()));
    let expectedQuery = "query dog($key: ID!)  {\ndog(key: $key)  {\nname  \nbreed  \ndescription  \n}\n\n}\n";
    let variables = Js.Dict.empty();
    Js.Dict.set(variables, "key", Js.Json.string("12345"));

    test("should return a valid urql query string", () =>
      Expect.(expect(myQuery->Query.queryGet) |> toEqual(expectedQuery))
    );

    test("should return the variables passed to the query", () =>
      Expect.(
        expect(myQuery->Query.variablesGet)
        |> toEqual(Some(Js.Json.object_(variables)))
      )
    );
  });

  describe("Make functor", () => {
    module GetDog = Query.Make(TestUtils.TestQueryWithVariable);
    let expectedQuery = "query dog($key: ID!)  {\ndog(key: $key)  {\nname  \nbreed  \ndescription  \n}\n\n}\n";
    let variables = Js.Dict.empty();
    Js.Dict.set(variables, "key", Js.Json.string("12345"));

    test("should return a valid urql query string", () =>
      Expect.(expect(GetDog.query->Query.queryGet) |> toEqual(expectedQuery))
    );

    test("should return an empty JS object for variables", () =>
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
          GetDog.queryFn(~variables=Js.Json.object_(variables), ())
          ->Query.variablesGet,
        )
        |> toEqual(Some(Js.Json.object_(variables)))
      )
    );
  });
});