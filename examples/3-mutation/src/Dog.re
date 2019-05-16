module Styles = {
  open Css;

  type colors = {halo: string};

  let colors = {halo: "222"};

  let wrapper =
    style([
      justifySelf(center),
      fontFamily("'Space Mono', monospace"),
      padding(px(12)),
    ]);

  let image =
    style([
      height(px(200)),
      width(px(200)),
      unsafe("object-fit", "cover"),
      borderRadius(pct(50.)),
      border(px(3), solid, hex(colors.halo)),
      boxShadows([
        boxShadow(~x=zero, ~y=zero, ~spread=px(3), rgba(0, 0, 0, 0.5)),
        boxShadow(~x=zero, ~y=zero, ~spread=px(6), rgba(0, 0, 0, 0.25)),
      ]),
    ]);

  let title = style([fontSize(rem(1.)), margin(px(0))]);
};

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Dog");

let make =
    (
      ~name: string,
      ~imageUrl: string,
      ~likes: int,
      ~pats: int,
      ~treats: int,
      ~bellyscratches: int,
      ~className: string,
      _children,
    ) => {
  ...component,
  render: _self => {
    <div className={Css.merge([Styles.wrapper, className])}>
      <img src=imageUrl alt=name className=Styles.image />
      <h3 className=Styles.title>
        {j|$name 👍$likes ✋$pats 🍖$treats 🐾$bellyscratches|j}->str
      </h3>
    </div>;
  },
};