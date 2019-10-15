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
      "should instantiate a client instance with fetchOptions provided as FetchOpts",
      () => {
      let client =
        Client.make(
          ~url="https://localhost:3000",
          ~fetchOptions=Client.FetchOpts(fetchOptions),
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

  describe("ssrExchange", () => {
    it("should exist and be callable", () =>
      Expect.(expect(Exchanges.ssrExchange()) |> toMatchSnapshot)
    );

    it(
      "should accept an initialState for passing data extracted during SSR prepass",
      () => {
      let json = Js.Dict.empty();
      Js.Dict.set(json, "key", Js.Json.number(1.));
      Js.Dict.set(json, "key2", Js.Json.number(2.));
      let data = Js.Json.object_(json);
      let serializedResult = Exchanges.serializedResult(~data, ());

      let initialState = Js.Dict.empty();
      Js.Dict.set(initialState, "query", serializedResult);
      let ssrExchangeOpts = Exchanges.ssrExchangeOpts(~initialState, ());

      Expect.(
        expect(() =>
          Exchanges.ssrExchange(~ssrExchangeOpts, ())
        )
        |> not
        |> toThrow
      );
    });

    it(
      "should expose an extractData method for extracting server-side rendered data",
      () => {
      let ssrCache = Exchanges.ssrExchange();

      Expect.(
        expect(() =>
          Exchanges.extractData(~exchange=ssrCache)
        )
        |> not
        |> toThrow
      );
    });

    it(
      "should expose a restoreData method for rehydrating data fetched server-side on the client",
      () => {
        let json = Js.Dict.empty();
        Js.Dict.set(json, "key", Js.Json.number(1.));
        Js.Dict.set(json, "key2", Js.Json.number(2.));
        let data = Js.Json.object_(json);
        let ssrCache = Exchanges.ssrExchange();

        Expect.(
          expect(() =>
            Exchanges.restoreData(~exchange=ssrCache, ~restore=data)
          )
          |> not
          |> toThrow
        );
      },
    );
  });
});
