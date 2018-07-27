type variables;

[@bs.deriving abstract]
type urqlMutation = {
  query: string,
  [@bs.optional]
  variables,
};

[@bs.module "urql"]
external mutation : (~query: string, ~variables: 'vars=?, unit) => urqlMutation =
  "";
