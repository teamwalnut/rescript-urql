open ReScriptUrql

let client = Client.make(~url="https://formidadog-ql.netlify.app/graphql", ())

switch ReactDOM.querySelector("#root") {
| Some(el) => ReactDOM.render(<Context.Provider value=client> <Grid /> </Context.Provider>, el)
| None => ()
}
