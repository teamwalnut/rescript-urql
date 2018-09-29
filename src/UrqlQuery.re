[@bs.deriving abstract]
type urqlQuery = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
};

[@bs.module "urql"]
external makeQuery:
  (~query: string, ~variables: Js.Json.t=?, unit) => urqlQuery =
  "query";

let query =
    (
      gqlQuery: {
        .
        "query": string,
        "variables": Js.Json.t,
        "parse": Js.Json.t => 'a,
      },
    ) =>
  makeQuery(~query=gqlQuery##query, ~variables=gqlQuery##variables, ());

module type GraphQLPPXInterface = {
  type t;
  let query: string;
};

module Make = (Query: GraphQLPPXInterface) => {
  let query = makeQuery(~query=Query.query, ());
  let queryFn = makeQuery(~query=Query.query);
};