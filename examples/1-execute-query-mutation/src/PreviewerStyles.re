open Css;

let previewer = style([display(flexBox), padding(px(20))]);

let side =
  style([
    display(flexBox),
    flexDirection(column),
    alignItems(center),
    flexBasis(pct(50.)),
  ]);

let section = style([margin(px(10))]);

let title =
  style([
    fontSize(rem(1.5)),
    fontFamily(`custom("'Space Mono', sans-serif")),
    margin(px(5)),
  ]);

let button = bdColor =>
  style([
    borderColor(hex(bdColor)),
    color(hex(bdColor)),
    backgroundColor(white),
    fontSize(rem(1.2)),
    margin(px(10)),
    borderRadius(px(25)),
    padding(px(10)),
    cursor(`pointer),
  ]);

let code = bgColor =>
  style([
    backgroundColor(hex(bgColor)),
    color(white),
    padding(px(20)),
    opacity(0.5),
    whiteSpace(`pre),
    fontFamily(`custom("'Space Mono', sans-serif")),
    borderRadius(px(5)),
    maxHeight(px(200)),
    overflow(`auto),
    width(vw(30.)),
  ]);

type colors = {
  query: string,
  mutation: string,
};

let colors = {query: "48a9dc", mutation: "db4d3f"};
