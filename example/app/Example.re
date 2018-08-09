open ReasonUrql;

/* Let's start by executing a simple query. */
let query: Query.urqlQuery =
  Query.query(
    ~query={j|
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

type fetchOptions = {. "formidable": int};

let fetchOptions = {"formidable": 1};

/* Create our client config. */
let config: Client.urqlClientConfig(fetchOptions) =
  Client.urqlClientConfig(
    ~url="http://localhost:3001",
    ~fetchOptions=`FetchObj(fetchOptions),
    (),
  );

/* Instantiate the client instance. */
let client = Client.client(config);

/* Cool, let's render the App now! */
ReactDOMRe.renderToElementWithId(<App />, "root");