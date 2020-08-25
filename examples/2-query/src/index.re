open ReasonUrql;

/* Instatiate the client. */
let client = Client.make(~url="https://graphql-pokemon.now.sh", ());

ReactDOMRe.renderToElementWithId(
  <Context.Provider value=client> <Container /> </Context.Provider>,
  "root",
);
