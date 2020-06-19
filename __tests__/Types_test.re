open Jest;

let it = test;

describe("Types", () => {
  describe("urqlResponseToReason", () => {
    it(
      "should correctly return Fetching constructor if fetching is true and no data has been received",
      () => {
        let response =
          Types.{fetching: true, data: None, error: None, extensions: None};
        let parse = _json => ();
        let result = Types.urqlResponseToReason(~response, ~parse);

        Expect.(expect(result.response) |> toEqual(Types.Fetching));
      },
    );

    it(
      "should return Data constructor if the GraphQL API responded with data",
      () => {
      let response =
        Types.{
          fetching: false,
          data: Some(Js.Json.string("Hello")),
          error: None,
          extensions: None,
        };
      let parse = json => Js.Json.decodeString(json);
      let result = Types.urqlResponseToReason(~response, ~parse);

      Expect.(
        expect(result.response) |> toEqual(Types.Data(Some("Hello")))
      );
    });

    it(
      "should return Error constructor if the GraphQL API responded with an error",
      () => {
      let errorJs = {
        "message": "Error returned by GraphQL API",
        "graphQLErrors": Js.Nullable.null,
        "networkError": Js.Nullable.null,
        "response": Js.Nullable.null,
      };

      let error =
        CombinedError.{
          message: "Error returned by GraphQL API",
          graphQLErrors: None,
          networkError: None,
          response: None,
        };

      let response =
        Types.{
          fetching: false,
          data: None,
          error: Some(errorJs),
          extensions: None,
        };
      let parse = _json => ();
      let result = Types.urqlResponseToReason(~response, ~parse);

      Expect.(expect(result.response) |> toEqual(Types.Error(error)));
    });

    it("should return NotFound constructor if none of the above apply", () => {
      let response =
        Types.{fetching: false, data: None, error: None, extensions: None};
      let parse = _json => ();
      let result = Types.urqlResponseToReason(~response, ~parse);

      Expect.(expect(result.response) |> toEqual(Types.NotFound));
    });
  })
});
