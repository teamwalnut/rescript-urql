open ReScriptUrql

/* This is the native debugExchange that ships with `urql`, re-implemented in ReScript.
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

switch ReactDOM.querySelector("#root") {
| Some(el) => ReactDOM.render(<Console client />, el)
| None => ()
}
