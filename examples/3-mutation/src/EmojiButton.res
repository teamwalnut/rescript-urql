@react.component
let make = (
  ~emoji: string,
  ~count: string,
  ~className: string,
  ~onClick: ReactEvent.Mouse.t => unit,
) =>
  <button className=j`emoji-button $className` onClick>
    <span className="emoji-button__text"> {count->React.string} </span>
    <span className="emoji-button__text"> {emoji->React.string} </span>
  </button>
