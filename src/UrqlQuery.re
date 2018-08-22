open UrqlInterfaces;

[@bs.deriving abstract]
type urqlQuery = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
};

[@bs.module "urql"]
external query : (~query: string, ~variables: Js.Json.t=?, unit) => urqlQuery =
  "";

module CreateQuery = (Query: GraphQLPPXInterface) => {
  let query: urqlQuery = query(~query=Query.query, ());
};