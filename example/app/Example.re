open ReasonUrql;

/* Let's start by executing a simple query. */
let query: Query.urqlQuery =
  Query.query(
    ~query=
      {|
query dogs {
  dogs {
    name
    breed
    description
  }
}
|},
    (),
  );

let makeFetchOptions =
  Fetch.RequestInit.make(
    ~method_=Post,
    ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
    (),
  );

let fetchOptions = Client.FetchObj(makeFetchOptions);

/* Instantiate the client instance. */
let client = Client.make(~url="http://localhost:3001", ~fetchOptions, ());

Client.executeQuery(~client, ~query, ~skipCache=false)
|> Js.Promise.then_(value => {
     let dogs = value##data##dogs;
     Js.log(dogs);
     Js.Promise.resolve(dogs);
   })
|> Js.Promise.catch(err => {
     Js.log2("Something went wrong!", err);
     Js.Promise.resolve(err);
   });

/* Cool, let's render the App now! */
ReactDOMRe.renderToElementWithId(<App client />, "root");