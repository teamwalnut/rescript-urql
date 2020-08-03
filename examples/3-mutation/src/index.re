open ReasonUrql;

let client =
  Client.make(~url="https://formidadog-ql.netlify.app/graphql", ());

ReactDOMRe.renderToElementWithId(
  <Context.Provider value=client> <Grid client /> </Context.Provider>,
  "root",
);
