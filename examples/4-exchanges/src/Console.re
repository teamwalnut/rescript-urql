module Styles = {
  open Css;

  type colors = {
    codeBackground: string,
    codeColor: string,
  };

  let colors = {codeBackground: "eff7ff", codeColor: "0366d6"};

  let wrapper =
    style([display(flexBox), height(vh(100.)), width(vw(100.))]);

  let title =
    style([
      fontFamily("'Space Mono', monospace"),
      fontSize(rem(3.)),
      margin(auto),
      padding(pct(10.)),
    ]);

  let code =
    style([
      backgroundColor(hex(colors.codeBackground)),
      color(hex(colors.codeColor)),
      padding(px(10)),
    ]);
};

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Console");

let make = _children => {
  ...component,
  render: _self =>
    <div className=Styles.wrapper>
      <h1 className=Styles.title>
        "Open your console to see the "->str
        <code className=Styles.code> "debugExchange"->str </code>
        " printing operations."->str
      </h1>
    </div>,
};