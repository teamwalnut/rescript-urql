[@bs.deriving abstract]
type urqlQuery = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
};

[@bs.module "urql"]
external query: (~query: string, ~variables: Js.Json.t) => urqlQuery = "";

let make =
    (
      gqlModule: {
        .
        "query": string,
        "variables": Js.Json.t,
        "parse": Js.Json.t => 'a,
      },
    ) =>
  query(~query=gqlModule##query, ~variables=gqlModule##variables);