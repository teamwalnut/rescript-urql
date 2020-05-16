module Styles = {
  open Css;

  let container =
    style([
      position(absolute),
      display(grid),
      gridTemplateColumns([rem(15.), `fr(1.)]),
      height(pct(100.)),
      width(pct(100.)),
      fontFamily(`custom("'Roboto', sans-serif")),
    ]);

  let aside =
    style([
      display(flexBox),
      flexDirection(column),
      height(pct(100.)),
      backgroundColor(hex("F25050")),
      padding(rem(0.5)),
      overflow(auto),
      boxSizing(`borderBox),
    ]);

  let search =
    style([
      marginBottom(rem(1.)),
      padding(rem(0.5)),
      border(rem(0.), `none, transparent),
    ]);

  let list =
    style([paddingLeft(rem(0.)), margin(rem(-0.5)), overflow(auto)]);

  let listItem =
    style([
      listStyleType(`none),
      padding3(~top=rem(0.5), ~h=rem(0.), ~bottom=rem(0.5)),
      hover([backgroundColor(hex("F29441"))]),
      cursor(`pointer),
    ]);

  let button =
    style([
      color(hex("ffff")),
      backgroundColor(transparent),
      borderColor(transparent),
      fontSize(rem(1.)),
      cursor(`pointer),
      width(pct(100.)),
      textAlign(`left),
    ]);
};
