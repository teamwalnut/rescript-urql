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

  let container =
    style([
      display(`flex),
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
      listStyle(`none, `outside, none),
      padding(px(5)),
      margin(px(10)),
      borderRadius(px(5)),
      fontSize(rem(2.)),
      fontFamily("'PT Serif Caption', serif"),
      width(pct(50.)),
      display(`flex),
      alignItems(center),
      justifyContent(center),
      animations([
        animation(~duration=500, ~fillMode=both, fadeIn),
        animation(
          ~duration=500,
          ~fillMode=both,
          ~direction=reverse,
          fadeIn,
          ~delay=4000,
        ),
      ]),
    ]);

  let gradientBlock =
    style([
      display(block),
      padding(px(10)),
      background(white),
      borderRadius(px(5)),
    ]);
};

let subscription = SubscribeHPMessages.make();
let query = subscription##query;

let component = ReasonReact.statelessComponent("Messages");

let make = _children => {
  ...component,
  render: _self =>
    <Subscription query>
      ...{({response}) =>
        switch (response) {
        | Fetching => <div> "Loading"->ReasonReact.string </div>
        | Data(d) =>
          <div className=Styles.container>
            <div key=d##newMessage##id className=Styles.message>
              <span className=Styles.gradientBlock>
                {d##newMessage##message->ReasonReact.string}
              </span>
            </div>
          </div>
        | Error(_e) => <div> "Error"->ReasonReact.string </div>
        | NotFound => <div> "Not Found"->ReasonReact.string </div>
        }
      }
    </Subscription>,
};