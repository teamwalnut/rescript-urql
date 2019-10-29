module Styles = {
  open Css;
  let dexContainer =
    style([
      display(flexBox),
      alignItems(flexStart),
      justifyContent(center),
      margin(auto),
      width(rem(25.)),
    ]);

  let dex =
    style([
      display(flexBox),
      flexDirection(column),
      width(pct(100.)),
      borderRadius(pct(5.)),
      background(
        linearGradient(
          deg(45),
          [(0, hex("F29441")), (100, hex("A04AD9"))],
        ),
      ),
      border(px(2), solid, hsl(210, 23, 95)),
      overflow(hidden),
      alignItems(center),
      boxShadow(~x=px(0), ~y=px(4), ~blur=px(6), hsl(0, 0, 80)),
    ]);

  let dexImageContainer =
    style([
      overflow(hidden),
      display(flexBox),
      justifyContent(center),
      alignItems(center),
      width(rem(12.)),
      height(rem(12.)),
      borderRadius(pct(100.)),
      backgroundColor(white),
      border(px(2), solid, black),
      margin(rem(2.)),
    ]);

  let dexImage = style([height(rem(8.))]);

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
    style([fontSize(rem(2.)), margin(zero), padding(rem(0.5))]);
  let dexSubTitle =
    style([fontSize(rem(1.5)), margin(zero), padding(rem(0.25))]);
  let dexGrid =
    style([display(grid), gridTemplateColumns([`fr(1.), `fr(1.)])]);
};
