[@bs.val] [@bs.module "urql"]
external createRequest:
  (~query: string, ~variables: Js.Json.t=?, unit) => UrqlTypes.graphqlRequest =
  "createRequest";
