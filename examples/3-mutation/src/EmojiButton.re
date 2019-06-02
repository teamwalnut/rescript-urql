[@react.component]
let make =
    (
      ~emoji: string,
      ~count: string,
      ~hex: string,
      ~onClick: ReactEvent.Mouse.t => unit,
    ) => {
  <button className={hex |> EmojiButtonStyles.emojiButton} onClick>
    <span className=EmojiButtonStyles.text> count->React.string </span>
    <span className=EmojiButtonStyles.text> emoji->React.string </span>
  </button>;
};