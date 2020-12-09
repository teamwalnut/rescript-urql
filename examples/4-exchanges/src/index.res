open ReasonUrql

/* This is the native debugExchange that ships with `urql`, re-implemented in Reason.
     Typically, you'd just add Exchanges.debugExchange to the Client's exchange array.
 */
let debugExchange = ({Client.Exchanges.forward: forward}, . ops) =>
  ops
  |> Wonka.tap((. op) => Js.log2("[debugExchange]: Incoming operation: ", op))
  |> forward
  |> Wonka.tap((. res) => Js.log2("[debugExchange]: Completed operation: ", res))

let client = Client.make(
  ~url="https://formidadog-ql.netlify.app/graphql",
  ~exchanges=[debugExchange, Client.Exchanges.fetchExchange],
  (),
)

ReactDOMRe.renderToElementWithId(<Console client />, "root")
