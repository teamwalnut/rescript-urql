/* Let's start by executing a simple query. */
let query: ReasonUrql.Query.urqlQuery =
  ReasonUrql.Query.query(
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
let config: ReasonUrql.Client.urqlClientConfig(fetchOptions) =
  ReasonUrql.Client.urqlClientConfig(
    ~url="http://localhost:3001",
    ~fetchOptions=`FetchObj(fetchOptions),
    (),
  );

/* Instantiate the client instance. */
let client = ReasonUrql.Client.client(config);

/* Cool, let's render the App now! */
ReactDOMRe.renderToElementWithId(<App />, "root");