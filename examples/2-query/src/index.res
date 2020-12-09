open ReasonUrql

/* Instatiate the client. */
let client = Client.make(~url="https://graphql-pokemon2.vercel.app", ())

ReactDOMRe.renderToElementWithId(
  <Context.Provider value=client> <Container /> </Context.Provider>,
  "root",
)
