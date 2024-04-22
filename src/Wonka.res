type t

module Source = {
  type t<'a>
}

module Subscription = {
  type t = {unsubscribe: unit => unit}
  module Consumer = {
    type t<'a> = Source.t<'a> => t
  }
}
module Signal = {
  type t<'a>
}

module Operator = {
  type t<'a, 'b> = Source.t<'a> => Source.t<'b>
}

module Sink = {
  type t<'a> = Signal.t<'a> => unit
}

@module("wonka")
external subscribe: ((. 'a) => unit) => Subscription.Consumer.t<'a> = "subscribe"

@module("wonka")
external map: ((. 'a) => 'b) => Operator.t<'a, 'b> = "map"

@module("wonka")
external take: int => Operator.t<'a, 'a> = "take"

@module("wonka")
external toPromise: Source.t<'a> => Js.Promise.t<'a> = "toPromise"

@module("wonka")
external fromValue: 'a => Source.t<'a> = "fromValue"

@module("wonka")
external fromPromise: Js.Promise.t<'a> => Source.t<'a> = "fromPromise"

@module("wonka")
external never: Source.t<'a> = "never"

@module("wonka")
external switchMap: ((. 'a) => Source.t<'b>) => Operator.t<'a, 'b> = "switchMap"
