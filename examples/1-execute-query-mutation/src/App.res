@react.component
let make = () =>
  <div
    style={ReactDOMStyle.make(
      ~position="absolute",
      ~top="0px",
      ~bottom="0px",
      ~left="0px",
      ~right="0px",
      ~padding="20px",
      (),
    )}>
    <Previewer />
  </div>
