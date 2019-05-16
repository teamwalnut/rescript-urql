open ReasonUrql;

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

let queryRequest = Request.createRequest(~query=GetAllDogs.make()##query, ());

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

Client.executeQuery(~client, ~query=queryRequest, ())
|> Wonka.subscribe((. response) => {
     let dogs = response##data##dogs;

     Js_global.setInterval(
       () => {
         dogs->Belt.Array.shuffleInPlace;

         let mutation = LikeDog.make(~key=dogs[0]##key, ());

         let mutationRequest =
           Request.createRequest(
             ~query=mutation##query,
             ~variables=mutation##variables,
             (),
           );

         Client.executeMutation(~client, ~mutation=mutationRequest, ())
         |> Wonka.subscribe((. response) => Js.log(response))
         |> ignore;
       },
       5000,
     )
     |> ignore;
   });

ReactDOMRe.renderToElementWithId(<Console />, "root");