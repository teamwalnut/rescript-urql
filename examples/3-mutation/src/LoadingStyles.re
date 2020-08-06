open Css;

let spin = keyframes([(100, [transform(rotate(deg(360.)))])]);

let loading =
  style([
    height(px(45)),
    before([
      unsafe("content", ""),
      position(absolute),
      top(pct(50.)),
      left(pct(50.)),
      width(px(20)),
      height(px(20)),
      marginTop(px(-10)),
      marginLeft(px(-10)),
      borderRadius(pct(50.)),
      border(px(1), solid, hex("f6f")),
      borderTopColor(hex("0e0")),
      borderRightColor(hex("0dd")),
      borderBottomColor(hex("f90")),
      animationName(spin),
      animationDuration(600),
      animationTimingFunction(linear),
      animationIterationCount(infinite),
    ]),
  ]);
