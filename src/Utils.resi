@module("@urql/core")
external stringifyVariablies: 'a => string = "stringifyVariables"

@module("@urql/core")
external createRequest: (~query: string, ~variables: Js.Json.t=?, unit) => Types.graphqlRequest =
  "createRequest"

@module("@urql/core")
external makeResult: (Types.operation, 'a, option<'b>) => Types.operationResultJs<'c> = "makeResult"

@module("@urql/core")
external makeErrorResult: (Types.operation, Js.Exn.t, option<'a>) => Types.operationResultJs<'c> =
  "makeErrorResult"

@module("@urql/core") external formatDocument: 'a => 'a = "formatDocument"

@module("@urql/core") external maskTypename: 'a => 'a = "maskTypename"
