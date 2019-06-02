open Css;

let grid =
  style([
    position(absolute),
    top(px(0)),
    bottom(px(0)),
    left(px(0)),
    right(px(0)),
    display(grid),
    gridTemplateRows([`fr(1.), `fr(1.), `fr(1.)]),
    gridTemplateColumns([`fr(1.), `fr(1.), `fr(1.), `fr(1.)]),
    media(
      "(max-width: 1070px)",
      [gridTemplateColumns([`fr(1.), `fr(1.), `fr(1.)])],
    ),
  ]);