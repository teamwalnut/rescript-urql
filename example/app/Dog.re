let component = ReasonReact.statelessComponent("Dog");

let make =
    (
      ~description: string,
      ~id: string,
      ~imageUrl: string,
      ~name: string,
      ~likes: int,
      ~onClick: {. "key": string} => unit,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div
      key=id
      style={
        ReactDOMRe.Style.make(
          ~padding="10px",
          ~fontFamily="Space Mono",
          ~cursor="pointer",
          (),
        )
      }
      onClick={_e => onClick({"key": id})}>
      <img
        style={ReactDOMRe.Style.make(~maxHeight="200px", ())}
        src=imageUrl
        alt=name
      />
      <div
        style={
          ReactDOMRe.Style.make(
            ~fontSize="20px",
            ~fontWeight="700",
            ~color="#222",
            ~borderBottom="1px solid #222",
            ~marginBottom="5px",
            ~display="flex",
            ~justifyContent="space-between",
            (),
          )
        }>
        <span> {ReasonReact.string(name)} </span>
        <span> {ReasonReact.string({j|$likes likes|j})} </span>
      </div>
      <div> {ReasonReact.string(description)} </div>
    </div>,
};