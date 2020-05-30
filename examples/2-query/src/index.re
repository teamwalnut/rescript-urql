open ReasonUrql;

/* Instatiate the client. */
let client = Client.make(~url="https://graphql-pokemon.now.sh", ());

ReactDOMRe.renderToElementWithId(
  <Provider value=client> <Container /> </Provider>,
  "root",
);
