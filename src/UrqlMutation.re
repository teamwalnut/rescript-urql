open UrqlInterfaces;

[@bs.deriving abstract]
type urqlMutation = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
};

[@bs.module "urql"]
external mutation:
  (~query: string, ~variables: Js.Json.t=?, unit) => urqlMutation =
  "";

module Make = (Mutation: GraphQLPPXInterface) => {
  let query = Mutation.query;
  let moduleMutation: urqlMutation = mutation(~query, ());
  let addMutationToMap = (~dict: Js.Dict.t(urqlMutation), ~key: string) =>
    Js.Dict.set(dict, key, moduleMutation);
};

let make =
    (
      gqlModule: {
        .
        "query": string,
        "variables": Js.Json.t,
        "parse": Js.Json.t => 'a,
      },
    ) =>
  mutation(~query=gqlModule##query, ~variables=gqlModule##variables);