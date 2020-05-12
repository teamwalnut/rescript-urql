open Jest;

let it = test;

describe("UrqlClientTypes", () => {
  describe("decodePartialOperationContext", () => {
    it("should create a partial operation context", () => {
      let partialOperationContext =
        UrqlClientTypes.partialOperationContext(
          ~url="https://new-endpoint.com/graphql",
          ~pollInterval=200,
          (),
        );

      let result =
        UrqlClientTypes.decodePartialOperationContext(
          Some(partialOperationContext),
        );

      let (url, pollInterval) = (
        result->Belt.Option.mapWithDefault(
          Some(""),
          UrqlClientTypes.PartialOperationContextJs.url,
        ),
        result->Belt.Option.mapWithDefault(
          Some(0),
          UrqlClientTypes.PartialOperationContextJs.pollInterval,
        ),
      );

      Expect.(
        expect((url, pollInterval))
        |> toEqual((Some("https://new-endpoint.com/graphql"), Some(200)))
      );
    });

    it(
      "should convert requestPolicy from its polymorphic variant representation to a string",
      () => {
        let partialOperationContext =
          UrqlClientTypes.partialOperationContext(
            ~requestPolicy=`NetworkOnly,
            (),
          );

        let result =
          UrqlClientTypes.decodePartialOperationContext(
            Some(partialOperationContext),
          );

        let requestPolicy =
          result->Belt.Option.mapWithDefault(
            Some(""),
            UrqlClientTypes.PartialOperationContextJs.requestPolicy,
          );

        Expect.(expect(requestPolicy) |> toEqual(Some("network-only")));
      },
    );
  });

  describe("createPartialOperationContext", () => {
    it(
      "should create a partialOperationContext with only the supplied fields",
      () => {
      let result =
        UrqlClientTypes.createOperationContext(
          ~requestPolicy=`NetworkOnly,
          ~url="https://new-endpoint.com/graphql",
          ~pollInterval=100,
          (),
        );

      Expect.(
        expect(result)
        |> toEqual(
             UrqlClientTypes.partialOperationContext(
               ~requestPolicy=`NetworkOnly,
               ~url="https://new-endpoint.com/graphql",
               ~pollInterval=100,
               (),
             ),
           )
      );
    })
  });
});
