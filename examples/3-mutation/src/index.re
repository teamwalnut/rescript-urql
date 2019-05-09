open ReasonUrql;

let client = Client.make(~url="https://formidadog-ql.now.sh", ());

ReactDOMRe.renderToElementWithId(
  <Provider value=client> <Grid client /> </Provider>,
  "root",
);