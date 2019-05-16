open ReasonUrql;
open SubscriptionsTransportWS;

let client =
  subscriptionClient(
    ~url="ws://localhost:4001/graphql",
    ~subscriptionClientConfig=subscriptionClientConfig(),
  );

let forwardSubscription = operation => client##request(operation);

let subscriptionExchangeOpts =
  Exchanges.subscriptionExchangeOpts(~forwardSubscription);

let subscriptionExchange =
  Exchanges.subscriptionExchange(subscriptionExchangeOpts);

let urqlClient =
  Client.make(
    ~url="http://localhost:4000/graphql",
    ~exchanges=
      Array.append(Exchanges.defaultExchanges, [|subscriptionExchange|]),
    (),
  );

ReactDOMRe.renderToElementWithId(
  <Provider value=urqlClient> <App /> </Provider>,
  "root",
);