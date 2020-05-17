open Jest;

let it = test;

describe("UrqlResponse", () => {
  /* Reused error Js.t and record across tests. */
  let errorJs = {
    "message": "Error returned by GraphQL API",
    "graphQLErrors": Js.Nullable.null,
    "networkError": Js.Nullable.null,
    "response": Js.Nullable.null,
  };

  let error =
    UrqlCombinedError.{
      message: "Error returned by GraphQL API",
      graphQLErrors: None,
      networkError: None,
      response: None,
    };

  describe("urqlResponseToReason", () => {
    it(
      "should correctly return Fetching constructor if fetching is true and no data has been received",
      () => {
        let response =
          UrqlTypes.{
            fetching: true,
            data: None,
            error: None,
            extensions: None,
          };
        let parse = _json => ();
        let result = UrqlResponse.urqlResponseToReason(~response, ~parse);

        Expect.(expect(result.response) |> toEqual(UrqlTypes.Fetching));
      },
    );

    it(
      "should return Data constructor if the GraphQL API responded with data",
      () => {
      let response =
        UrqlTypes.{
          fetching: false,
          data: Some(Js.Json.string("Hello")),
          error: None,
          extensions: None,
        };
      let parse = json => Js.Json.decodeString(json);
      let result = UrqlResponse.urqlResponseToReason(~response, ~parse);

      Expect.(
        expect(result.response) |> toEqual(UrqlTypes.Data(Some("Hello")))
      );
    });

    it(
      "should return Error constructor if the GraphQL API responded with an error",
      () => {
      let response =
        UrqlTypes.{
          fetching: false,
          data: None,
          error: Some(errorJs),
          extensions: None,
        };
      let parse = _json => ();
      let result = UrqlResponse.urqlResponseToReason(~response, ~parse);

      Expect.(expect(result.response) |> toEqual(UrqlTypes.Error(error)));
    });

    it("should return NotFound constructor if none of the above apply", () => {
      let response =
        UrqlTypes.{
          fetching: false,
          data: None,
          error: None,
          extensions: None,
        };
      let parse = _json => ();
      let result = UrqlResponse.urqlResponseToReason(~response, ~parse);

      Expect.(expect(result.response) |> toEqual(UrqlTypes.NotFound));
    });
  });

  describe("urqlClientResponseToReason", () => {
    /* Reused operation records across tests. */
    let operationContext =
      UrqlClientTypes.{
        fetchOptions: None,
        requestPolicy: `CacheFirst,
        url: "https://localhost:3000/graphql",
        meta: None,
        pollInterval: None,
      };

    let operation =
      UrqlClientTypes.{
        key: 1,
        query: "query { \n dogs \n }",
        variables: None,
        operationName: `Query,
        context: operationContext,
      };

    it(
      "should return Data constructor if the GraphQL API responded with data",
      () => {
      let response =
        UrqlClientTypes.{
          data: Some(Js.Json.string("Hello")),
          error: None,
          operation,
        };
      let parse = json => Js.Json.decodeString(json);
      let result = UrqlResponse.urqlClientResponseToReason(~response, ~parse);

      Expect.(
        expect(result.response)
        |> toEqual(UrqlClientTypes.Data(Some("Hello")))
      );
    });

    it(
      "should return Error constructor if the GraphQL API responded with data",
      () => {
      let response =
        UrqlClientTypes.{data: None, error: Some(errorJs), operation};
      let parse = json => Js.Json.decodeString(json);
      let result = UrqlResponse.urqlClientResponseToReason(~response, ~parse);

      Expect.(
        expect(result.response) |> toEqual(UrqlClientTypes.Error(error))
      );
    });

    it("should return NotFound constructor if none of the above apply", () => {
      let response = UrqlClientTypes.{data: None, error: None, operation};
      let parse = _json => ();
      let result = UrqlResponse.urqlClientResponseToReason(~response, ~parse);

      Expect.(expect(result.response) |> toEqual(UrqlClientTypes.NotFound));
    });
  });
});
