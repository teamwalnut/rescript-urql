module Styles = {
  open Css;
  let dexContainer =
    style([
      display(flexBox),
      alignItems(flexStart),
      justifyContent(center),
      paddingTop(em(1.)),
    ]);

  let dex =
    style([
      display(flexBox),
      flexDirection(column),
      borderRadius(pct(5.)),
      backgroundColor(red),
      border(px(2), solid, hsl(210, 23, 95)),
      overflow(hidden),
      width(pct(50.)),
      alignItems(center),
      boxShadow(~x=px(0), ~y=px(4), ~blur=px(6), hsl(0, 0, 80)),
    ]);

  let dexImageContainer =
    style([
      overflow(hidden),
      display(flexBox),
      justifyContent(center),
      alignItems(center),
      width(em(8.)),
      height(em(8.)),
      borderRadius(pct(100.)),
      backgroundColor(white),
      border(px(2), solid, black),
      margin(em(3.)),
    ]);
  let dexImage =
    style([overflow(hidden), width(em(6.)), height(em(6.))]);

  let dexText =
    style([
      display(flexBox),
      flexDirection(column),
      backgroundColor(white),
      width(pct(100.)),
      borderBottomLeftRadius(pct(5.0)),
      borderBottomRightRadius(pct(5.0)),
      borderTop(px(2), solid, hsl(210, 23, 95)),
      alignItems(center),
    ]);

  let dexTitle =
    style([fontSize(em(2.)), margin(zero), padding(em(0.5))]);
  let dexSubTitle =
    style([fontSize(em(1.5)), margin(zero), padding(em(0.25))]);
  let dexGrid =
    style([display(grid), gridTemplateColumns([`fr(1.), `fr(1.)])]);
};
