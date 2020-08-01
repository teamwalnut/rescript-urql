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

type t = {
  message: string,
  locations: option(array(sourceLocation)),
  path: option(array(path)),
  nodes: option(array(astNode)),
  source: option(source),
  positions: option(array(int)),
  originalError: option(Js.Exn.t),
  extensions: option(Js.Dict.t(extension)),
};
