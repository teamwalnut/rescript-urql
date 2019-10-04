open ReasonUrql;
open Client;

/* This is the native debugExchange that ships with `urql`, re-implemented in Reason.
     Typically, you'd just add Exchanges.debugExchange to the Client's exchange array.
   */
let debugExchange = exchangeInput => {
  let forward = Exchanges.forwardGet(exchangeInput);

  ops =>
    ops
    |> Wonka.tap((. op) =>
         Js.log2("[debugExchange]: Incoming operation: ", op)
       )
    |> forward
    |> Wonka.tap((. res) =>
         Js.log2("[debugExchange]: Completed operation: ", res)
       );
};

let client =
  Client.make(
    ~url="https://formidadog-ql.now.sh",
    ~exchanges=[|debugExchange, Exchanges.fetchExchange|],
    (),
  );

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

let queryRequest = GetAllDogs.make();

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

executeQuery(~client, ~request=queryRequest, ())
|> Wonka.subscribe((. data) =>
     switch (data.response) {
     | Data(d) =>
       Js_global.setInterval(
         () => {
           d##dogs->Belt.Array.shuffleInPlace;
           let mutationRequest = LikeDog.make(~key=d##dogs[0]##key, ());
           executeMutation(~client, ~request=mutationRequest, ())
           |> Wonka.subscribe((. response) => Js.log(response))
           |> ignore;
         },
         5000,
       )
       |> ignore
     | _ => ()
     }
   );

ReactDOMRe.renderToElementWithId(<Console />, "root");
