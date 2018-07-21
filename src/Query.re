type variables;

[@bs.deriving abstract]
type urqlQuery = {
  query: string,
  [@bs.optional]
  variables
};

[@bs.module "urql"]
external query : (~query: string, ~variables: 'vars=?, unit) => urqlQuery = "";
