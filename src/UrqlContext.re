[@bs.module "urql"]
external context: React.Context.t(UrqlClient.t) = "Context";

module Provider = {
  [@bs.module "urql"] [@react.component]
  external make:
    (~value: UrqlClient.t, ~children: React.element) => React.element =
    "Provider";
}

module Consumer = {
  [@bs.module "urql"] [@react.component]
  external make:
    (~children: UrqlClient.t => React.element) => React.element =
    "Consumer";
}
