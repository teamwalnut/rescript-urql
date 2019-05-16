module Styles = {
  open Css;

  let coldStart =
    style([fontFamily("'Space Mono', monospace"), fontSize(rem(1.5))]);
};

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("ColdStart");

let make = _children => {
  ...component,
  render: _self => {
    <h1 className=Styles.coldStart>
      "The deployed server is a Now instance booting from a cold start. Wait ~30 seconds after executing your first query to see results. After that, you should see updates instantly!"
      ->str
    </h1>;
  },
};