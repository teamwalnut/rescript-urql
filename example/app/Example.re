/* Let's start by executing a simple query. */
let query: Query.urqlQuery =
  Query.query(
    ~query=
      {j|
query {
  dogs {
    name
    breed
    description
  }
}
|j},
    (),
  );

type fetchOptions = {.
  "formidable": int
};

let fetchOptions = {
  "formidable": 1
};
let fetchFunc = `FetchFunc(() => {"formidable": 5000 });
/* Create our client config. */
let config: Client.urqlClientConfig(fetchOptions) =
  Client.urqlClientConfig(~url="http://localhost:3001", ~fetchOptions=`FetchObj(fetchOptions), ());

/* Instantiate the client instance. */
let client = Client.client(config);

/* Execute a query! */
Client.executeQuery(client, query, false)
|> Js.Promise.then_(value => {
     let dogs = value##data##dogs;
     Js.log(dogs);
     Js.Promise.resolve(dogs);
   });

/* Cool, let's render the App now! */
ReactDOMRe.renderToElementWithId(<App />, "root");