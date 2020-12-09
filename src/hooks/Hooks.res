include UseClient
include UseQuery
include UseMutation
include UseSubscription

type hookResponse<'ret> = Types.Hooks.hookResponse<'ret> = {
  operation: Types.operation,
  fetching: bool,
  data: option<'ret>,
  error: option<CombinedError.t>,
  response: Types.Hooks.response<'ret>,
  extensions: option<Js.Json.t>,
  stale: bool,
}
