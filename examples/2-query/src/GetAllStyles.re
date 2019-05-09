module Styles = {
  open Css;

  let container =
    style([
      position(absolute),
      display(grid),
      gridTemplateColumns([`fr(0.3), `fr(1.)]),
      height(pct(100.)),
      width(pct(100.)),
      fontFamily("Roboto"),
    ]);

  let search =
    style([
      display(flexBox),
      flexDirection(column),
      overflow(scroll),
      backgroundColor(rgba(0, 0, 0, 0.3)),
    ]);

  let searchBox =
    style([
      margin(em(1.5)),
      padding(em(0.4)),
      borderRadius(em(2.)),
      border(em(0.), `none, transparent),
    ]);

  let list = style([listStyleType(`none), margin(em(0.2))]);

  let listButton =
    style([
      color(hex("ffff")),
      backgroundColor(transparent),
      borderColor(transparent),
    ]);
};