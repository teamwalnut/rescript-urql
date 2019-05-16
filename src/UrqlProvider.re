[@bs.module "urql"] external provider: ReasonReact.reactClass = "Provider";
let component = ReasonReact.statelessComponent("Provider");

let make = (~value: UrqlClient.t, children) =>
  ReasonReact.wrapJsForReason(~reactClass=provider, ~props={"value": value}, children);