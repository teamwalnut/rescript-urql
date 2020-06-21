open ReasonUrql;

let client = Client.make(~url="https://formidadog-ql.now.sh", ());

ReactDOMRe.renderToElementWithId(
  <Context.Provider value=client> <Grid client /> </Context.Provider>,
  "root",
);
