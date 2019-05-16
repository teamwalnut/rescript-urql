let component = ReasonReact.statelessComponent("App");

module Styles = {
  open Css;

  let page =
    style([
      display(flexBox),
      flexDirection(column),
      position(absolute),
      top(px(0)),
      bottom(px(0)),
      left(px(0)),
      right(px(0)),
      padding(px(20)),
    ]);

  let side =
    style([
      display(flexBox),
      flexDirection(column),
      alignItems(center),
      flexBasis(pct(50.)),
      firstChild([justifyContent(flexEnd)]),
    ]);
};

let make = _children => {
  ...component,
  render: _self =>
    <div className=Styles.page>
      <section className=Styles.side> <Logo /> </section>
      <section className=Styles.side> <Messages /> </section>
    </div>,
};