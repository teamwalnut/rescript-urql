open ReasonUrql;

module SubscribeHPMessages = [%graphql
  {|
  subscription subscribeMessages {
    newMessage {
      id
      message
    }
  }
|}
];

module Styles = {
  open Css;

  let wrapper =
    style([
      display(flexBox),
      flexDirection(column),
      alignItems(center),
      justifyContent(center),
    ]);

  let fadeIn =
    keyframes([
      (0, [transform(translateY(px(20))), opacity(0.)]),
      (100, [transform(translateY(px(0))), opacity(1.)]),
    ]);

  let message =
    style([
      background(linearGradient(turn(0.25), [(0, red), (100, orange)])),
      listStyle(none, `outside, none),
      padding(px(5)),
      margin(px(10)),
      borderRadius(px(5)),
      fontSize(rem(2.)),
      fontFamily("'PT Serif Caption', serif"),
      width(pct(50.)),
      display(flexBox),
      alignItems(center),
      justifyContent(center),
      animation(~duration=500, ~fillMode=both, fadeIn),
    ]);

  let gradientBlock =
    style([
      display(block),
      padding(px(10)),
      background(white),
      borderRadius(px(5)),
    ]);
};

let str = ReasonReact.string;

let subscription = SubscribeHPMessages.make();
let query = subscription##query;

let component = ReasonReact.statelessComponent("Messages");

let make = _children => {
  ...component,
  render: _self =>
    <Subscription query>
      ...{({response}) =>
        switch (response) {
        | Fetching => <div> "Loading"->str </div>
        | Data(d) =>
          <div className=Styles.wrapper>
            <div key=d##newMessage##id className=Styles.message>
              <span className=Styles.gradientBlock>
                {d##newMessage##message->str}
              </span>
            </div>
          </div>
        | Error(_e) => <div> "Error"->str </div>
        | NotFound => <div> "Not Found"->str </div>
        }
      }
    </Subscription>,
};