open ReasonUrql;

module SubscribeRandomFloat = [%graphql
  {|
  subscription subscribeFloat {
    newFloat @bsDecoder(fn: "Js.Float.toString")
  }
|}
];

let handler = (prevSubscriptions, subscription) => {
  switch (prevSubscriptions) {
  | Some(subs) => Array.append(subs, [|subscription|])
  | None => [|subscription|]
  };
};

[@bs.scope "Math"] [@bs.val] external random: unit => float = "random";
[@bs.scope "Math"] [@bs.val] external floor: float => int = "floor";
[@bs.send] external toString: (int, int) => string = "toString";

let getRandomInt = (max: int) => {
  floor(random() *. float_of_int(max));
};

let getRandomHex = () => {
  let encode = random() *. float_of_int(16777215) |> floor;
  let hex = encode->toString(16);
  {j|#$hex|j};
};

let request = SubscribeRandomFloat.make();

[@react.component]
let make = () => {
  let (Hooks.{response}, _) =
    Hooks.useSubscription(~request, ~handler=Handler(handler), ());

  switch (response) {
  | Fetching => <text> "Loading"->React.string </text>
  | Data(d) =>
    Array.mapi(
      (index, datum) =>
        <rect
          key={datum##newFloat ++ string_of_int(index)}
          x={datum##newFloat}
          y={index === 0 ? datum##newFloat : d[index - 1]##newFloat}
          stroke={getRandomHex()}
          fill="none"
          height={getRandomInt(30) |> string_of_int}
          width={getRandomInt(30) |> string_of_int}
        />,
      d,
    )
    |> React.array
  | Error(_e) => <text> "Error"->React.string </text>
  | NotFound => <text> "Not Found"->React.string </text>
  };
};
