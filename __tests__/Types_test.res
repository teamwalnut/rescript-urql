open Jest

let it = test

let mockOperationContext: Types.operationContext = {
  additionalTypenames: None,
  fetch: None,
  fetchOptions: None,
  requestPolicy: #CacheFirst,
  url: "https://localhost:3000/graphql",
  meta: None,
  suspense: Some(false),
  preferGetMethod: Some(false),
}

let mockOperation = {
  open Types
  {
    key: 1,
    query: "query {\ndogs {\nname\nlikes\n}\n}",
    variables: None,
    kind: #Query,
    context: mockOperationContext,
  }
}

describe("Types", () =>
  describe("hookResponseToReScript", () => {
    it(
      "should correctly return Fetching constructor if fetching is true and no data has been received",
      () => {
        let response = {
          open Types.Hooks
          {
            operation: mockOperation,
            fetching: true,
            data: Js.Nullable.undefined,
            error: None,
            extensions: None,
            stale: false,
          }
        }
        let parse = _json => ()
        let result = Types.Hooks.hookResponseToReScript(~response, ~parse)

        open Expect
        expect(result.response) -> toEqual(Types.Hooks.Fetching)
      },
    )

    it("should return Data constructor if the GraphQL API responded with data", () => {
      let response = {
        open Types.Hooks
        {
          operation: mockOperation,
          fetching: false,
          data: Js.Nullable.return(Js.Json.string("Hello")),
          error: None,
          extensions: None,
          stale: false,
        }
      }
      let parse = json => Js.Json.decodeString(json)
      let result = Types.Hooks.hookResponseToReScript(~response, ~parse)

      open Expect
      expect(result.response) -> toEqual(Types.Hooks.Data(Some("Hello")))
    })

    it(
      "should return PartialData constructor if the GraphQL API responded with data and an error",
      () => {
        let graphQLError = {
          open GraphQLError
          {
            message: "Error encountered querying field dogs.",
            locations: Some([{line: 2, column: 17}]),
            path: None,
            source: None,
            nodes: None,
            positions: None,
            originalError: None,
            extensions: None,
          }
        }

        let error = {
          open CombinedError
          {
            name: "CombinedError",
            message: "Error returned by GraphQL API",
            graphQLErrors: [graphQLError],
            networkError: None,
            response: None,
            toString: () => "Error returned by GraphQL API",
          }
        }

        let response = {
          open Types.Hooks
          {
            operation: mockOperation,
            fetching: false,
            data: Js.Nullable.return(Js.Json.string("Hello")),
            error: Some(error),
            extensions: None,
            stale: false,
          }
        }

        let parse = json => Js.Json.decodeString(json)
        let result = Types.Hooks.hookResponseToReScript(~response, ~parse)

        open Expect
        expect(result.response) -> toEqual(
          Types.Hooks.PartialData(Some("Hello"), error.graphQLErrors),
        )
      },
    )

    it("should return Error constructor if the GraphQL API responded with an error", () => {
      let graphQLError = {
        open GraphQLError
        {
          message: "Error encountered querying field dogs.",
          locations: Some([{line: 2, column: 17}]),
          path: None,
          source: None,
          nodes: None,
          positions: None,
          originalError: None,
          extensions: None,
        }
      }

      let error = {
        open CombinedError
        {
          name: "CombinedError",
          message: "Error returned by GraphQL API",
          graphQLErrors: [graphQLError],
          networkError: None,
          response: None,
          toString: () => "Error returned by GraphQL API",
        }
      }

      let response = {
        open Types.Hooks
        {
          operation: mockOperation,
          fetching: false,
          data: Js.Nullable.undefined,
          error: Some(error),
          extensions: None,
          stale: false,
        }
      }
      let parse = _json => ()
      let result = Types.Hooks.hookResponseToReScript(~response, ~parse)

      open Expect
      expect(result.response) -> toEqual(Types.Hooks.Error(error))
    })

    it("should return Empty constructor if none of the above apply", () => {
      let response = {
        open Types.Hooks
        {
          operation: mockOperation,
          fetching: false,
          data: Js.Nullable.undefined,
          error: None,
          extensions: None,
          stale: false,
        }
      }
      let parse = _json => ()
      let result = Types.Hooks.hookResponseToReScript(~response, ~parse)

      open Expect
      expect(result.response) -> toEqual(Types.Hooks.Empty)
    })
  })
)
