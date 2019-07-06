open Css;

let console =
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
    backgroundColor(hex("eff7ff")),
    color(hex("0366d6")),
    padding(px(10)),
  ]);
