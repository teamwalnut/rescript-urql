[@bs.module "urql"]
external stringifyVariablies: 'a => string = "stringifyVariables";

[@bs.module "urql"]
external createRequest:
  (~query: string, ~variables: Js.Json.t=?, unit) => Types.graphqlRequest =
  "createRequest";

[@bs.module "urql"]
external makeResult:
  (Types.operation, 'a, option('b)) => Types.operationResult('c) =
  "makeResult";

[@bs.module "urql"]
external makeErrorResult:
  (Types.operation, Js.Exn.t, option('a)) => Types.operationResult('c) =
  "makeErrorResult";

[@bs.module "urql"] external formatDocument: 'a => 'a = "formatDocument";

[@bs.module "urql"] external maskTypename: 'a => 'a = "maskTypename";
