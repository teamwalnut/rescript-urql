@module("urql") external context: React.Context.t<Client.t> = "Context"

module Provider: {
  @module("urql") @react.component
  external make: (~value: Client.t, ~children: React.element) => React.element = "Provider"
}

module Consumer: {
  @module("urql") @react.component
  external make: (~children: Client.t => React.element) => React.element = "Consumer"
}
