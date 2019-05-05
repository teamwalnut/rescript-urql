open Jest;
open ReasonUrql;

let it = test;

describe("UrqlClient", () => {
  describe("Client with only a url provided", () => {
    let client = Client.make(~url="https://localhost:3000", ());

    it("should instantiate a client instance", () =>
      Expect.(expect(client) |> toMatchSnapshot)
    );

    it("should expose an executeQuery method", () =>
      ExpectJs.(expect(Client.executeQuery) |> toBeTruthy)
    );

    it("should expose an executeMutation method", () =>
      ExpectJs.(expect(Client.executeMutation) |> toBeTruthy)
    );

    it("should expose an executeSubscription method", () =>
      ExpectJs.(expect(Client.executeSubscription) |> toBeTruthy)
    );

    it("should expose an executeRequestOperation method", () =>
      ExpectJs.(expect(Client.executeRequestOperation) |> toBeTruthy)
    );

    it("should expose an reexecuteOperation method", () =>
      ExpectJs.(expect(Client.reexecuteOperation) |> toBeTruthy)
    );

    it("should expose an createRequestOperation method", () =>
      ExpectJs.(expect(Client.createRequestOperation) |> toBeTruthy)
    );

    it("should expose an dispatchOperation method", () =>
      ExpectJs.(expect(Client.dispatchOperation) |> toBeTruthy)
    );
  });

  describe("Client with fetchOptions provided", () => {
    let fetchOptions =
      Fetch.RequestInit.make(
        ~method_=Post,
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        (),
      );

    it(
      "should instantiate a client instance with fetchOptions provided as FetchObj",
      () => {
      let client =
        Client.make(
          ~url="https://localhost:3000",
          ~fetchOptions=Client.FetchObj(fetchOptions),
          (),
        );

      Expect.(expect(client) |> toMatchSnapshot);
    });

    it(
      "should instantiate a client instance with fetchOptions provided as FetchFn",
      () => {
      let client =
        Client.make(
          ~url="https://localhost:3000",
          ~fetchOptions=Client.FetchFn(() => fetchOptions),
          (),
        );

      Expect.(expect(client) |> toMatchSnapshot);
    });
  });

  describe("Client with exchanges provided", () => {
    it("should instantiate a client with exchanges", () => {
      let client =
        Client.make(
          ~url="https://localhost:3000",
          ~exchanges=[|Exchanges.debugExchange|],
          (),
        );

      Expect.(expect(client) |> toMatchSnapshot);
    });

    it("should allow a user to compose exchanges into a single exchange", () =>
      Expect.(
        expect(() =>
          [|
            Exchanges.debugExchange,
            Exchanges.cacheExchange,
            Exchanges.fetchExchange,
          |]
          |> Exchanges.composeExchanges
        )
        |> not
        |> toThrow
      )
    );

    it("should return a single exchange from compose exchanges", () =>
      Expect.(
        expect([|
          [|
            Exchanges.debugExchange,
            Exchanges.cacheExchange,
            Exchanges.fetchExchange,
          |]
          |> Exchanges.composeExchanges,
        |])
        |> toHaveLength(1)
      )
    );
  });
});