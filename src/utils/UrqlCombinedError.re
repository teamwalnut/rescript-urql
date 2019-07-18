/* A message describing the Error for debugging purposes. */
type message = string;

[@bs.deriving abstract]
type sourceLocation = {
  line: int,
  column: int,
};

/* An array of { line, column } locations within the source GraphQL document
   which correspond to this error. */
type locations = array(sourceLocation);

/* Abstract types representing path and astNode.
   path – The JSON-path into the execution response which corresponds to this error.
   astNode - A GraphQL AST node corresponding to this error. */
type path;
type astNode;

/* The source GraphQL document corresponding to this error. */
[@bs.deriving abstract]
type source = {
  body: string,
  name: string,
  locationOffset: sourceLocation,
};

/* An array of character offsets within the source GraphQL document which correspond to this error. */
type positions = array(int);

/* The original error thrown from a field resolver during execution. */
type originalError = Js.Exn.t;

/* Extension fields to add to the formatted error. */
type extension;

/* A simple binding to the GraphQL error type exposed by graphql-js. See:
    https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/types/graphql/error/GraphQLError.d.ts.
   */
[@bs.deriving abstract]
type graphqlError = {
  message: Js.Nullable.t(message),
  locations: Js.Nullable.t(locations),
  path: Js.Nullable.t(array(path)),
  nodes: Js.Nullable.t(array(astNode)),
  source: Js.Nullable.t(source),
  positions: Js.Nullable.t(positions),
  originalError: Js.Nullable.t(originalError),
  extensions: Js.Nullable.t(Js.Dict.t(extension)),
};

class type _combinedError =
  [@bs]
  {
    pub networkError: Js.Nullable.t(Js.Exn.t);
    pub graphqlErrors: Js.Nullable.t(array(graphqlError));
    pub response: 'response;
  };

type t = Js.t(_combinedError);

[@bs.module "urql"] external combinedError: t = "CombinedError";
