open ReasonUrql

let client = Client.make(~url="https://graphql-pokemon2.vercel.app", ())

switch ReactDOM.querySelector("#root") {
| Some(el) => ReactDOM.render(<Context.Provider value=client> <Container /> </Context.Provider>, el)
| None => ()
}
