[@bs.deriving abstract]
type urqlMutation = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
};

[@bs.module "urql"]
external makeMutation:
  (~query: string, ~variables: Js.Json.t=?, unit) => urqlMutation =
  "mutation";

/* Expose a function that can make a mutation from a graphql_ppx module when variables
   argument is known before runtime. */
let mutation =
    (
      gqlMutation: {
        .
        "query": string,
        "variables": Js.Json.t,
        "parse": Js.Json.t => 'a,
      },
    ) =>
  makeMutation(
    ~query=gqlMutation##query,
    ~variables=gqlMutation##variables,
    (),
  );

/* Also expose a functor that can make a mutation from a graphql_ppx module when variables
   argument is not known before runtime. */
module type GraphQLPPXInterface = {
  type t;
  let query: string;
};

module Make = (Mutation: GraphQLPPXInterface) => {
  let mutation = makeMutation(~query=Mutation.query, ());

  let mutationFn = makeMutation(~query=Mutation.query);
};