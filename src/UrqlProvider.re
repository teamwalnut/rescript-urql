[@bs.module "urql"] [@react.component]
external make:
  (~value: UrqlClient.t, ~children: React.element) => React.element =
  "Provider";