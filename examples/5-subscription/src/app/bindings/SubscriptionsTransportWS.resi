open ReasonUrql

type subscriptionClientOptions = {
  timeout: option<int>,
  @bs.as("lazy")
  lazy_: option<bool>,
  reconnect: option<bool>,
  reconnectionAttempts: option<int>,
  connectionCallback: option<Js.Exn.t => unit>,
  inactivityTimer: option<int>,
}

type websocketImpl

type subscriptionClientConfig = {
  options: option<subscriptionClientOptions>,
  websocketImpl: option<websocketImpl>,
}

let makeClientOptions: (
  ~timeout: int=?,
  ~lazy_: bool=?,
  ~reconnect: bool=?,
  ~reconnectionAttempts: int=?,
  ~connectionCallback: Js.Exn.t => unit=?,
  ~inactivityTimer: int=?,
  unit,
) => subscriptionClientOptions

type t = {
  @bs.meth
  "request": Client.Exchanges.subscriptionOperation => Client.Exchanges.observableLike<
    Types.executionResult,
  >,
}

let make: (~url: string, ~subscriptionClientConfig: subscriptionClientOptions=?, unit) => t
