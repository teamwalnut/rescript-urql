[@react.component]
let make = () =>
  <div className=AppStyles.page>
    <svg
      viewBox="0 0 1000 1000"
      height="1000"
      width="1000"
      style={ReactDOMRe.Style.make(~border="1px solid blue", ())}>
      <Messages />
    </svg>
  </div>;