open ReasonUrql;
open Hooks;

module SubscribeRandomInt = [%graphql
  {|
  subscription subscribeNumbers {
    newNumber @bsDecoder(fn: "string_of_int")
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

[@react.component]
let make = () => {
  let {response} =
    useSubscription(
      ~request=SubscribeRandomInt.make(),
      ~handler=Handler(handler),
      ()
    );

  switch (response) {
  | Fetching => <text> "Loading"->React.string </text>
  | Data(d) =>
    d
    |> Array.mapi((index, datum) =>
         <circle
           cx=datum##newNumber
           cy={index === 0 ? datum##newNumber : d[index - 1]##newNumber}
           stroke={getRandomHex()}
           fill="none"
           r={getRandomInt(30) |> string_of_int}
         />
       )
    |> React.array
  | Error(_e) => <text> "Error"->React.string </text>
  | NotFound => <text> "Not Found"->React.string </text>
  };
};
