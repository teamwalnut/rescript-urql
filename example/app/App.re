open ReasonUrql;

/* Now, let's render a child component attached to a query. First, wrap
   the component in <Provider />. */
let component = ReasonReact.statelessComponent("App");

let make = (~client: Client.client, _children) => {
  ...component,
  render: _self => <Provider client> <Header /> <DogList /> </Provider>,
};