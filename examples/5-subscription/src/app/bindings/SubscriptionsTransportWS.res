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

let makeClientOptions = (
  ~timeout=?,
  ~lazy_=?,
  ~reconnect=?,
  ~reconnectionAttempts=?,
  ~connectionCallback=?,
  ~inactivityTimer=?,
  (),
) => {
  timeout: timeout,
  lazy_: lazy_,
  reconnect: reconnect,
  reconnectionAttempts: reconnectionAttempts,
  connectionCallback: connectionCallback,
  inactivityTimer: inactivityTimer,
}

type t = {
  @bs.meth
  "request": Client.Exchanges.subscriptionOperation => Client.Exchanges.observableLike<
    Types.executionResult,
  >,
}

@bs.new @bs.module("subscriptions-transport-ws")
external subscriptionClient: (
  ~url: string,
  ~subscriptionClientConfig: subscriptionClientConfig=?,
  unit,
) => t = "SubscriptionClient"

let make = (~url, ~subscriptionClientConfig=?, ()) =>
  switch subscriptionClientConfig {
  | Some(config) =>
    let {
      timeout,
      lazy_,
      reconnect,
      reconnectionAttempts,
      connectionCallback,
      inactivityTimer,
    } = config

    let clientOptions = makeClientOptions(
      ~timeout?,
      ~lazy_?,
      ~reconnect?,
      ~reconnectionAttempts?,
      ~connectionCallback?,
      ~inactivityTimer?,
      (),
    )
    subscriptionClient(
      ~url,
      ~subscriptionClientConfig={
        options: Some(clientOptions),
        websocketImpl: None,
      },
      (),
    )
  | None => subscriptionClient(~url, ())
  }
