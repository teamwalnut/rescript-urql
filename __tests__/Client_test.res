open Jest

let it = test

describe("Client", () => {
  describe("Client with only a url provided", () => {
    let client = Client.make(~url="https://localhost:3000", ())

    it(
      "should instantiate a client instance",
      () => {
        open Expect
        expect(client) |> toMatchSnapshot
      },
    )

    it(
      "should expose an executeQuery method",
      () => {
        open ExpectJs
        expect(Client.executeQuery) |> toBeTruthy
      },
    )

    it(
      "should expose an executeMutation method",
      () => {
        open ExpectJs
        expect(Client.executeMutation) |> toBeTruthy
      },
    )

    it(
      "should expose an executeSubscription method",
      () => {
        open ExpectJs
        expect(Client.executeSubscription) |> toBeTruthy
      },
    )
  })

  describe("Client with fetchOptions provided", () => {
    let fetchOptions: Fetch.Request.init = {
      method: #POST,
      headers: Fetch.Headers.fromObject({"Content-Type": "application/json"}),
    }

    it(
      "should instantiate a client instance with fetchOptions provided as FetchOpts",
      () => {
        let client = Client.make(
          ~url="https://localhost:3000",
          ~fetchOptions=Client.FetchOpts(fetchOptions),
          (),
        )

        open Expect
        expect(client) |> toMatchSnapshot
      },
    )

    it(
      "should instantiate a client instance with fetchOptions provided as FetchFn",
      () => {
        let client = Client.make(
          ~url="https://localhost:3000",
          ~fetchOptions=Client.FetchFn(() => fetchOptions),
          (),
        )

        open Expect
        expect(client) |> toMatchSnapshot
      },
    )
  })

  describe("Client with default requestPolicy", () =>
    it(
      "should accept a default requestPolicy",
      () => {
        let client = Client.make(
          ~url="https://localhost:3000",
          ~requestPolicy=#"cache-and-network",
          (),
        )

        open Expect
        expect(client) |> toMatchSnapshot
      },
    )
  )

  describe("Client with suspense flag", () =>
    it(
      "should accept a suspense flag to enable experimental SSR mode",
      () => {
        let client = Client.make(~url="https://localhost:3000", ~suspense=true, ())

        open Expect
        expect(client) |> toMatchSnapshot
      },
    )
  )

  describe("Client with exchanges provided", () => {
    it(
      "should instantiate a client with exchanges",
      () => {
        let client = Client.make(
          ~url="https://localhost:3000",
          ~exchanges=[Client.Exchanges.debugExchange],
          (),
        )

        open Expect
        expect(client) |> toMatchSnapshot
      },
    )

    it(
      "should allow a user to compose exchanges into a single exchange",
      () => {
        open Expect
        expect(
          () =>
            [
              Client.Exchanges.debugExchange,
              Client.Exchanges.cacheExchange,
              Client.Exchanges.fetchExchange,
            ] |> Client.Exchanges.composeExchanges,
        )
        |> not_
        |> toThrow
      },
    )

    it(
      "should return a single exchange from compose exchanges",
      () => {
        open Expect
        expect([
          [
            Client.Exchanges.debugExchange,
            Client.Exchanges.cacheExchange,
            Client.Exchanges.fetchExchange,
          ] |> Client.Exchanges.composeExchanges,
        ])->toHaveLength(1)
      },
    )
  })

  describe("ssrExchange", () => {
    it(
      "should exist and be callable",
      () => {
        open Expect
        expect(Client.Exchanges.ssrExchange()) |> toMatchSnapshot
      },
    )

    it(
      "should accept an initialState for passing data extracted during SSR prepass",
      () => {
        let json = Js.Dict.empty()
        Js.Dict.set(json, "key", Js.Json.number(1.))
        Js.Dict.set(json, "key2", Js.Json.number(2.))
        let data = Js.Json.object_(json)
        let serializedResult = {
          open Client.Exchanges
          {data: Some(data), error: None}
        }

        let initialState = Js.Dict.empty()
        Js.Dict.set(initialState, "query", serializedResult)
        let ssrExchangeParams = {
          open Client.Exchanges
          {
            initialState: Some(initialState),
            isClient: Some(false),
          }
        }

        open Expect
        expect(() => Client.Exchanges.ssrExchange(~ssrExchangeParams, ())) |> not_ |> toThrow
      },
    )

    it(
      "should expose an extractData method for extracting server-side rendered data",
      () => {
        let ssrCache = Client.Exchanges.ssrExchange()

        open Expect
        expect(() => Client.Exchanges.extractData(~exchange=ssrCache)) |> not_ |> toThrow
      },
    )

    it(
      "should expose a restoreData method for rehydrating data fetched server-side on the client",
      () => {
        let json = Js.Dict.empty()
        Js.Dict.set(json, "key", Js.Json.number(1.))
        Js.Dict.set(json, "key2", Js.Json.number(2.))
        let data = Js.Json.object_(json)
        let ssrCache = Client.Exchanges.ssrExchange()

        open Expect
        expect(() => Client.Exchanges.restoreData(~exchange=ssrCache, ~restore=data))
        |> not_
        |> toThrow
      },
    )
  })

  describe("with custom fetch implementation", () => {
    it(
      "should accept a custom fetch implementation",
      () => {
        let fetcher = url => Fetch.fetch(url, {})
        let client = Client.make(
          ~url="http://localhost:3000",
          ~fetch=Client.FetchWithUrl(fetcher),
          (),
        )
        open Expect
        expect(client) |> toMatchSnapshot
      },
    )

    it(
      "should work with a fetcher using Fetch.Request.t",
      () => {
        let fetcher = request =>
          switch Fetch.Request.destination(request) {
          | #audio => Fetch.send(Fetch.Request.make("/audio/graphql", {}))
          | _ => Fetch.send(Fetch.Request.make("/graphql", {}))
          }

        let client = Client.make(
          ~url="http://localhost:3000",
          ~fetch=Client.FetchWithRequest(fetcher),
          (),
        )
        open Expect
        expect(client) |> toMatchSnapshot
      },
    )
  })

  describe("with preferGetMethod", () =>
    it(
      "respects the preferGetMethod flag",
      () => {
        let client = Client.make(~url="https://localhost:3000", ~preferGetMethod=true, ())

        open Expect
        expect(client) |> toMatchSnapshot
      },
    )
  )

  describe("with maskTypename", () =>
    it(
      "respects the maskTypename flag",
      () => {
        let client = Client.make(~url="https://localhost:3000", ~maskTypename=true, ())

        open Expect
        expect(client) |> toMatchSnapshot
      },
    )
  )

  describe("Ecosystem exchanges", () => {
    describe(
      "persistedFetchExchange",
      () => {
        it(
          "should return None for all persistedFetchExchange options if unspecified",
          () => {
            let persistedFetchExchangeOptions = Client.Exchanges.makePersistedFetchExchangeOptions()

            open Expect
            expect(persistedFetchExchangeOptions)->toEqual({
              Client.Exchanges.preferGetForPersistedQueries: None,
              generateHash: None,
            })
          },
        )

        it(
          "should apply any specified options to the persistedFetchExchange",
          () => {
            let hashFn = (query, _) => Js.Promise.resolve(Js.String.normalize(query))

            let persistedFetchExchangeOptions = Client.Exchanges.makePersistedFetchExchangeOptions(
              ~preferGetForPersistedQueries=true,
              ~generateHash=hashFn,
              (),
            )

            open Expect
            expect(persistedFetchExchangeOptions)->toEqual({
              Client.Exchanges.preferGetForPersistedQueries: Some(true),
              generateHash: Some(hashFn),
            })
          },
        )
      },
    )

    describe(
      "requestPolicyExchange",
      () => {
        it(
          "should return None for all requestPolicyExchange options if unspecified",
          () => {
            let requestPolicyExchangeOptions = Client.Exchanges.makeRequestPolicyExchangeOptions()

            open Expect
            expect(requestPolicyExchangeOptions)->toEqual({
              Client.Exchanges.shouldUpgrade: None,
              ttl: None,
            })
          },
        )

        it(
          "should apply any specified options to the requestPolicyExchange",
          () => {
            let shouldUpgrade = (operation: Types.operation) =>
              operation.context.requestPolicy !== #"cache-only"

            let requestPolicyExchangeOptions = Client.Exchanges.makeRequestPolicyExchangeOptions(
              ~shouldUpgrade,
              ~ttl=2000,
              (),
            )

            open Expect
            expect(requestPolicyExchangeOptions)->toEqual({
              Client.Exchanges.shouldUpgrade: Some(shouldUpgrade),
              ttl: Some(2000),
            })
          },
        )
      },
    )

    describe(
      "retryExchange",
      () => {
        it(
          "should apply the default retryExchange options from urql if none are applied",
          () => {
            let retryExchangeOptions = Client.Exchanges.makeRetryExchangeOptions()

            open Expect
            expect(retryExchangeOptions)->toEqual({
              Client.Exchanges.initialDelayMs: None,
              maxDelayMs: None,
              maxNumberAttempts: None,
              randomDelay: None,
              retryIf: None,
            })
          },
        )

        it(
          "should apply any specified options to the retryExchange",
          () => {
            let retryExchangeOptions = Client.Exchanges.makeRetryExchangeOptions(
              ~initialDelayMs=200,
              ~randomDelay=false,
              (),
            )

            open Expect
            expect(retryExchangeOptions)->toEqual({
              Client.Exchanges.initialDelayMs: Some(200),
              maxDelayMs: None,
              maxNumberAttempts: None,
              randomDelay: Some(false),
              retryIf: None,
            })
          },
        )
      },
    )
  })
})
