open ReasonUrql;

module GetAllDogs = [%graphql
  {|
  query dogs {
    dogs {
      key
      name
      breed
      likes
    }
  }
|}
];

module LikeDog = [%graphql
  {|
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      name
      likes
    }
  }
  |}
];

[@react.component]
let make = (~client) => {
  React.useEffect1(
    () => {
      let mutSub = ref(Wonka_types.{unsubscribe: () => ()});

      let subscription =
        Client.executeQuery(
          ~client,
          ~request=(module GetAllDogs),
          ~variables=GetAllDogs.makeVariables(),
          (),
        )
        |> Wonka.subscribe((. data) =>
             switch (Client.(data.response)) {
             | Data(d) =>
               Js_global.setInterval(
                 () => {
                   GetAllDogs.(d.dogs)->Belt.Array.shuffleInPlace;

                   let mutationSubscription =
                     Client.executeMutation(
                       ~client,
                       ~request=(module LikeDog),
                       ~variables=
                         LikeDog.makeVariables(~key=d.dogs[0].key, ()),
                       (),
                     )
                     |> Wonka.subscribe((. response) => Js.log(response));

                   mutSub := mutationSubscription;
                   ();
                 },
                 5000,
               )
               |> ignore
             | _ => ()
             }
           );

      Some(
        () => {
          subscription.unsubscribe();
          mutSub^.unsubscribe();
        },
      );
    },
    [|client|],
  );

  <div className="console">
    <h1 className="console__title">
      "Open your console to see the "->React.string
      <code className="console__code"> "debugExchange"->React.string </code>
      " printing operations."->React.string
    </h1>
  </div>;
};
