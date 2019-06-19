open Css;

let page =
  style([
    display(flexBox),
    flexDirection(column),
    alignItems(center),
    justifyContent(center),
    position(absolute),
    top(px(0)),
    bottom(px(0)),
    left(px(0)),
    right(px(0)),
    padding(px(20)),
  ]);