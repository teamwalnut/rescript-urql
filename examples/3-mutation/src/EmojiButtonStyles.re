open Css;

let emojiButton = color =>
  style([
    borderRadius(pct(50.)),
    padding(px(5)),
    border(px(2), solid, hex(color)),
    display(flexBox),
    alignItems(center),
    justifyContent(spaceEvenly),
    margin(px(5)),
    selector(":hover", [backgroundColor(hex(color)), cursor(`pointer)]),
    selector(":first-child", [marginLeft(px(0))]),
  ]);

let text =
  style([paddingLeft(px(2)), paddingRight(px(2)), fontSize(rem(1.))]);
