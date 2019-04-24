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

module Styles = {
  open Css;

  let container =
    style([
      display(`flex),
      flexDirection(column),
      alignItems(center),
      justifyContent(center),
      position(absolute),
      top(px(0)),
      left(px(0)),
      right(px(0)),
      bottom(px(0)),
    ]);
};

ReactDOMRe.renderToElementWithId(
  <Provider value=urqlClient>
    <div className=Styles.container> <Logo /> <Messages /> </div>
  </Provider>,
  "root",
);