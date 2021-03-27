open ReScriptUrql

module SubscribeRandomFloat = %graphql(`
  subscription subscribeFloat {
    newFloat
  }
`)

let handler = (prevSubscriptions, subscription) =>
  switch prevSubscriptions {
  | Some(subs) => Array.append(subs, [subscription])
  | None => [subscription]
  }

@react.component
let make = () => {
  let ({Hooks.response: response}, _) = Hooks.useSubscription(
    ~subscription=module(SubscribeRandomFloat),
    ~handler=Handler(handler),
    (),
  )

  switch response {
  | Fetching => <text> {"Loading"->React.string} </text>
  | Data(d)
  | PartialData(d, _) =>
    Array.mapi(
      (index, datum: SubscribeRandomFloat.t) =>
        <rect
          key={datum.newFloat->Js.Float.toString ++ string_of_int(index)}
          x={datum.newFloat->Js.Float.toString}
          y={(index === 0 ? datum.newFloat : d[index - 1].newFloat)->Js.Float.toString}
          stroke="none"
          fill={Util.getRandomHex()}
          fillOpacity="0.5"
          height={Util.getRandomInt(30) |> string_of_int}
          width={Util.getRandomInt(30) |> string_of_int}
        />,
      d,
    ) |> React.array
  | Error(_e) => <text> {"Error"->React.string} </text>
  | Empty => <text> {"Not Found"->React.string} </text>
  }
}
