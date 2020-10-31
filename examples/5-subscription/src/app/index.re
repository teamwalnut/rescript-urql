open ReasonUrql;

let subscriptionClient =
  SubscriptionsTransportWS.make(
    ~url="ws://localhost:4001/graphql",
    ~subscriptionClientConfig=SubscriptionsTransportWS.makeClientOptions(),
    (),
  );

let forwardSubscription = operation => subscriptionClient##request(operation);

let client =
  Client.make(
    ~url="http://localhost:4000/graphql",
    ~exchanges=
      Array.append(
        Client.Exchanges.defaultExchanges,
        [|Client.Exchanges.subscriptionExchange(~forwardSubscription, ())|],
      ),
    (),
  );

ReactDOMRe.renderToElementWithId(
  <Context.Provider value=client> <App /> </Context.Provider>,
  "root",
);
