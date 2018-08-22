module type GraphQLPPXInterface = {
  let query: string;
  type t;
  let parse: Js.Json.t => t;
};