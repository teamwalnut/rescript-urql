open ReasonUrql;

let client =
  SubscriptionsTransportWS.make(
    ~url="ws://localhost:4001/graphql",
    ~subscriptionClientConfig=SubscriptionsTransportWS.makeClientOptions(),
    (),
  );

let forwardSubscription = operation => client##request(operation);

let subscriptionExchangeOpts =
  Client.Exchanges.{forwardSubscription: forwardSubscription};

let subscriptionExchange =
  Client.Exchanges.subscriptionExchange(subscriptionExchangeOpts);

let urqlClient =
  Client.make(
    ~url="http://localhost:4000/graphql",
    ~exchanges=
      Array.append(
        Client.Exchanges.defaultExchanges,
        [|subscriptionExchange|],
      ),
    (),
  );

ReactDOMRe.renderToElementWithId(
  <Context.Provider value=urqlClient> <App /> </Context.Provider>,
  "root",
);
