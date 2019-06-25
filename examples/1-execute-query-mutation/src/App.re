module Styles = {
  open Css;

  let page =
    style([
      position(absolute),
      top(px(0)),
      bottom(px(0)),
      left(px(0)),
      right(px(0)),
      padding(px(20)),
    ]);
};

[@react.component]
let make = () => {
  <div className=Styles.page> <ColdStart /> <Previewer /> </div>;
};
