type t;

[@bs.deriving abstract]
type error = {
  [@bs.optional]
  networkError: Js.Exn.t,
};

[@bs.new] [@bs.module "urql"]
external combinedError: string => t = "CombinedError";