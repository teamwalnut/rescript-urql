/* let read = (~query) => Js.Promise.make((~resolve, ~reject as _) => resolve(. { "blah": 2 }));
let write = (~query, ~data) => Js.Promise.make((~resolve, ~reject as _) => resolve(. 4));
let cache: Client.urqlCache = Client.urqlCache(~read=read, ~write=write); */

/* Setup the config for the client. */
let config: Client.urqlClientConfig({.}) =
  Client.urqlClientConfig(~url="http://localhost:3001", ());

/* Instantiate the client instance. */
let client = Client.client(config);

/* Now, let's render a child component attached to a query. First, wrap
   the component in <Provider />. */
let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => <Provider client> <Header /> <DogList /> </Provider>,
};
