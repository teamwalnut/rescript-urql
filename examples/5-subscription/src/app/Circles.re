open ReasonUrql;

module SubscribeRandomInt = [%graphql
  {|
  subscription subscribeNumbers {
    newNumber
  }
|}
];

let handler = (prevSubscriptions, subscription) => {
  switch (prevSubscriptions) {
  | Some(subs) => Array.append(subs, [|subscription|])
  | None => [|subscription|]
  };
};

[@react.component]
let make = () => {
  let (Hooks.{response}, _) =
    Hooks.useSubscription(
      ~query=(module SubscribeRandomInt),
      ~handler=Handler(handler),
      (),
    );

  switch (response) {
  | Fetching => <text> "Loading"->React.string </text>
  | Data(d)
  | PartialData(d, _) =>
    d
    |> Array.to_list
    |> List.mapi((index, datum: SubscribeRandomInt.t) => {
         let cx = datum.newNumber->string_of_int;
         let cy =
           (index === 0 ? datum.newNumber : d[index - 1].newNumber)
           ->string_of_int;
         <circle
           key={string_of_int(index)}
           cx
           cy
           stroke={Util.getRandomHex()}
           fill="none"
           r={Util.getRandomInt(30) |> string_of_int}
         />;
       })
    |> Array.of_list
    |> React.array
  | Error(_e) => <text> "Error"->React.string </text>
  | Empty => <text> "Not Found"->React.string </text>
  };
};
