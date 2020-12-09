@module("urql")
external stringifyVariablies: 'a => string = "stringifyVariables"

@module("urql")
external createRequest: (~query: string, ~variables: Js.Json.t=?, unit) => Types.graphqlRequest =
  "createRequest"

@module("urql")
external makeResult: (Types.operation, 'a, option<'b>) => Types.operationResultJs<'c> = "makeResult"

@module("urql")
external makeErrorResult: (Types.operation, Js.Exn.t, option<'a>) => Types.operationResultJs<'c> =
  "makeErrorResult"

@module("urql") external formatDocument: 'a => 'a = "formatDocument"

@module("urql") external maskTypename: 'a => 'a = "maskTypename"
