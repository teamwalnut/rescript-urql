class type _combinedError =
  [@bs]
  {
    pub networkError: Js.Nullable.t(Js.Exn.t);
    pub graphQLErrors: array(GraphQLError.t);
    pub response: Js.Nullable.t(Fetch.response);
    pub message: string;
  };

type combinedErrorJs = Js.t(_combinedError);
[@bs.module "urql"] external combinedError: combinedErrorJs = "CombinedError";

type combinedError = {
  networkError: option(Js.Exn.t),
  graphQLErrors: array(GraphQLError.t),
  response: option(Fetch.response),
  message: string,
};

let combinedErrorToRecord = (err: combinedErrorJs): combinedError => {
  {
    networkError: err##networkError->Js.Nullable.toOption,
    graphQLErrors: err##graphQLErrors,
    response: err##response->Js.Nullable.toOption,
    message: err##message,
  };
};

type t = combinedError;
