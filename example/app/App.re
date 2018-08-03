/* Setup the config for the client. */
let config: ReasonUrql.Client.urqlClientConfig({.}) =
  ReasonUrql.Client.urqlClientConfig(~url="http://localhost:3001", ());

/* Instantiate the client instance. */
let client = ReasonUrql.Client.client(config);

/* Now, let's render a child component attached to a query. First, wrap
   the component in <Provider />. */
let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <ReasonUrql.Provider client> <Header /> <DogList /> </ReasonUrql.Provider>,
};