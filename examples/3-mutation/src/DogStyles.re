open Css;

let container =
  style([
    justifySelf(center),
    fontFamily(`custom("'Space Mono', monospace")),
    padding(px(12)),
  ]);

let image =
  style([
    height(px(200)),
    width(px(200)),
    unsafe("object-fit", "cover"),
    borderRadius(pct(50.)),
    border(px(3), solid, hex("222")),
    boxShadows([
      Shadow.box(~x=zero, ~y=zero, ~spread=px(3), rgba(0, 0, 0, 0.5)),
      Shadow.box(~x=zero, ~y=zero, ~spread=px(6), rgba(0, 0, 0, 0.25)),
    ]),
  ]);

let title = style([fontSize(rem(1.)), margin(px(0))]);

let buttons =
  style([display(flexBox), alignItems(center), position(relative)]);
