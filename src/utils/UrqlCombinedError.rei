type sourceLocation = {
  line: int,
  column: int,
};

type path;
type astNode;

type source = {
  body: string,
  name: string,
  locationOffset: sourceLocation,
};

type extension;

type graphQLError = {
  message: string,
  locations: option(array(sourceLocation)),
  path: option(array(path)),
  nodes: option(array(astNode)),
  source: option(source),
  positions: option(array(int)),
  originalError: option(Js.Exn.t),
  extensions: option(Js.Dict.t(extension)),
};

class type _combinedError =
  [@bs]
  {
    pub networkError: Js.Nullable.t(Js.Exn.t);
    pub graphQLErrors: Js.Nullable.t(array(graphQLError));
    pub response: Js.Nullable.t(Fetch.response);
    pub message: string;
  };

type combinedErrorJs = Js.t(_combinedError);
[@bs.module "urql"] external combinedError: combinedErrorJs = "CombinedError";

type combinedError = {
  networkError: option(Js.Exn.t),
  graphQLErrors: option(array(graphQLError)),
  response: option(Fetch.response),
  message: string,
};

let combinedErrorToRecord: combinedErrorJs => combinedError;

type t = combinedError;
