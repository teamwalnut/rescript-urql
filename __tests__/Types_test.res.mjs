// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Js_json from "rescript/lib/es6/js_json.js";
import * as Jest$ReScriptUrql from "../src/Jest.res.mjs";
import * as Types$ReScriptUrql from "../src/Types.res.mjs";

var mockOperationContext = {
  additionalTypenames: undefined,
  fetch: undefined,
  fetchOptions: undefined,
  requestPolicy: "cache-first",
  url: "https://localhost:3000/graphql",
  meta: undefined,
  suspense: false,
  preferGetMethod: false
};

var mockOperation = {
  key: 1,
  query: "query {\ndogs {\nname\nlikes\n}\n}",
  variables: undefined,
  kind: "query",
  context: mockOperationContext
};

Jest$ReScriptUrql.describe("Types", (function (param) {
        Jest$ReScriptUrql.describe("hookResponseToReScript", (function (param) {
                Jest$ReScriptUrql.test("should correctly return Fetching constructor if fetching is true and no data has been received", (function (param) {
                        var response = {
                          operation: mockOperation,
                          fetching: true,
                          data: undefined,
                          error: undefined,
                          extensions: undefined,
                          stale: false
                        };
                        var parse = function (_json) {
                          
                        };
                        var result = Types$ReScriptUrql.Hooks.hookResponseToReScript(response, parse);
                        return Jest$ReScriptUrql.Expect.toEqual(Jest$ReScriptUrql.Expect.expect(result.response), "Fetching");
                      }));
                Jest$ReScriptUrql.test("should return Data constructor if the GraphQL API responded with data", (function (param) {
                        var response = {
                          operation: mockOperation,
                          fetching: false,
                          data: "Hello",
                          error: undefined,
                          extensions: undefined,
                          stale: false
                        };
                        var parse = Js_json.decodeString;
                        var result = Types$ReScriptUrql.Hooks.hookResponseToReScript(response, parse);
                        return Jest$ReScriptUrql.Expect.toEqual(Jest$ReScriptUrql.Expect.expect(result.response), {
                                    TAG: "Data",
                                    _0: "Hello"
                                  });
                      }));
                Jest$ReScriptUrql.test("should return PartialData constructor if the GraphQL API responded with data and an error", (function (param) {
                        var graphQLError_locations = [{
                            line: 2,
                            column: 17
                          }];
                        var graphQLError = {
                          message: "Error encountered querying field dogs.",
                          locations: graphQLError_locations,
                          path: undefined,
                          nodes: undefined,
                          source: undefined,
                          positions: undefined,
                          originalError: undefined,
                          extensions: undefined
                        };
                        var error_graphQLErrors = [graphQLError];
                        var error_toString = function (param) {
                          return "Error returned by GraphQL API";
                        };
                        var error = {
                          name: "CombinedError",
                          message: "Error returned by GraphQL API",
                          graphQLErrors: error_graphQLErrors,
                          networkError: undefined,
                          response: undefined,
                          toString: error_toString
                        };
                        var response_error = error;
                        var response = {
                          operation: mockOperation,
                          fetching: false,
                          data: "Hello",
                          error: response_error,
                          extensions: undefined,
                          stale: false
                        };
                        var parse = Js_json.decodeString;
                        var result = Types$ReScriptUrql.Hooks.hookResponseToReScript(response, parse);
                        return Jest$ReScriptUrql.Expect.toEqual(Jest$ReScriptUrql.Expect.expect(result.response), {
                                    TAG: "PartialData",
                                    _0: "Hello",
                                    _1: error_graphQLErrors
                                  });
                      }));
                Jest$ReScriptUrql.test("should return Error constructor if the GraphQL API responded with an error", (function (param) {
                        var graphQLError_locations = [{
                            line: 2,
                            column: 17
                          }];
                        var graphQLError = {
                          message: "Error encountered querying field dogs.",
                          locations: graphQLError_locations,
                          path: undefined,
                          nodes: undefined,
                          source: undefined,
                          positions: undefined,
                          originalError: undefined,
                          extensions: undefined
                        };
                        var error_graphQLErrors = [graphQLError];
                        var error_toString = function (param) {
                          return "Error returned by GraphQL API";
                        };
                        var error = {
                          name: "CombinedError",
                          message: "Error returned by GraphQL API",
                          graphQLErrors: error_graphQLErrors,
                          networkError: undefined,
                          response: undefined,
                          toString: error_toString
                        };
                        var response_error = error;
                        var response = {
                          operation: mockOperation,
                          fetching: false,
                          data: undefined,
                          error: response_error,
                          extensions: undefined,
                          stale: false
                        };
                        var parse = function (_json) {
                          
                        };
                        var result = Types$ReScriptUrql.Hooks.hookResponseToReScript(response, parse);
                        return Jest$ReScriptUrql.Expect.toEqual(Jest$ReScriptUrql.Expect.expect(result.response), {
                                    TAG: "Error",
                                    _0: error
                                  });
                      }));
                Jest$ReScriptUrql.test("should return Empty constructor if none of the above apply", (function (param) {
                        var response = {
                          operation: mockOperation,
                          fetching: false,
                          data: undefined,
                          error: undefined,
                          extensions: undefined,
                          stale: false
                        };
                        var parse = function (_json) {
                          
                        };
                        var result = Types$ReScriptUrql.Hooks.hookResponseToReScript(response, parse);
                        return Jest$ReScriptUrql.Expect.toEqual(Jest$ReScriptUrql.Expect.expect(result.response), "Empty");
                      }));
              }));
      }));

var it = Jest$ReScriptUrql.test;

export {
  it ,
  mockOperationContext ,
  mockOperation ,
}
/*  Not a pure module */
