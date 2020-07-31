open Jest;

let it = test;

describe("Types", () => {
  describe("urqlResponseToReason", () => {
    it(
      "should correctly return Fetching constructor if fetching is true and no data has been received",
      () => {
        let response =
          Types.{
            fetching: true,
            data: None,
            error: None,
            extensions: None,
            stale: false,
          };
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
          stale: false,
        };
      let parse = json => Js.Json.decodeString(json);
      let result = Types.urqlResponseToReason(~response, ~parse);

      Expect.(
        expect(result.response) |> toEqual(Types.Data(Some("Hello")))
      );
    });

    it(
      "should return PartialData constructor if the GraphQL API responded with data and an error",
      () => {
        let graphQLError =
          GraphQLError.{
            message: "Error encountered querying field dogs.",
            locations: Some([|{line: 2, column: 17}|]),
            path: None,
            source: None,
            nodes: None,
            positions: None,
            originalError: None,
            extensions: None,
          };

        let errorJs = {
          "message": "Error returned by GraphQL API",
          "graphQLErrors": [|graphQLError|],
          "networkError": Js.Nullable.null,
          "response": Js.Nullable.null,
        };

        let error =
          CombinedError.{
            message: "Error returned by GraphQL API",
            graphQLErrors: [|graphQLError|],
            networkError: None,
            response: None,
          };

        let response =
          Types.{
            fetching: false,
            data: Some(Js.Json.string("Hello")),
            error: Some(errorJs),
            extensions: None,
            stale: false,
          };

        let parse = json => Js.Json.decodeString(json);
        let result = Types.urqlResponseToReason(~response, ~parse);

        Expect.(
          expect(result.response)
          |> toEqual(Types.PartialData(Some("Hello"), error.graphQLErrors))
        );
      },
    );

    it(
      "should return Error constructor if the GraphQL API responded with an error",
      () => {
      let graphQLError =
        GraphQLError.{
          message: "Error encountered querying field dogs.",
          locations: Some([|{line: 2, column: 17}|]),
          path: None,
          source: None,
          nodes: None,
          positions: None,
          originalError: None,
          extensions: None,
        };

      let errorJs = {
        "message": "Error returned by GraphQL API",
        "graphQLErrors": [|graphQLError|],
        "networkError": Js.Nullable.null,
        "response": Js.Nullable.null,
      };

      let error =
        CombinedError.{
          message: "Error returned by GraphQL API",
          graphQLErrors: [|graphQLError|],
          networkError: None,
          response: None,
        };

      let response =
        Types.{
          fetching: false,
          data: None,
          error: Some(errorJs),
          extensions: None,
          stale: false,
        };
      let parse = _json => ();
      let result = Types.urqlResponseToReason(~response, ~parse);

      Expect.(expect(result.response) |> toEqual(Types.Error(error)));
    });

    it("should return Empty constructor if none of the above apply", () => {
      let response =
        Types.{
          fetching: false,
          data: None,
          error: None,
          extensions: None,
          stale: false,
        };
      let parse = _json => ();
      let result = Types.urqlResponseToReason(~response, ~parse);

      Expect.(expect(result.response) |> toEqual(Types.Empty));
    });
  })
});
