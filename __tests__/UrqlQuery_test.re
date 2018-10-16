open Jest;
open ReasonUrql;

describe("UrqlQuery", () => {
  describe("UrqlQuery with only query specified", () => {
    module TestQuery = [%graphql
      {|
        query dogs {
          dogs {
            name
            breed
            description
          }
        }|}
    ];

    let myQuery = Query.query(TestQuery.make());
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
    module TestQuery = [%graphql
      {|
        query dog($key: ID!) {
          dog(key: $key) {
            name
            breed
            description
          }
        }|}
    ];

    let myQuery = Query.query(TestQuery.make(~key="12345", ()));
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
});