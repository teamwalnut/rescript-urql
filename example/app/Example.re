open ReasonUrql;

module GetAllDogs = [%graphql
  {|
query dogs {
  dogs {
    name
    breed
    description
  }
}|}
];
let queryAllDogs = Query.make(GetAllDogs.make());

module GetDog = [%graphql
  {|
query dog($key: ID!) {
  dog(key: $key) {
    name
    breed
  }
}
|}
];
let queryOneDog = Query.make(GetDog.make(~key="VmeRTX7j-", ()));

let makeFetchOptions =
  Fetch.RequestInit.make(
    ~method_=Post,
    ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
    (),
  );

let fetchOptions = Client.FetchObj(makeFetchOptions);

/* Instantiate the client instance. */
let client = Client.make(~url="http://localhost:3001", ~fetchOptions, ());

Client.executeQuery(~client, ~query=queryAllDogs, ~skipCache=false)
|> Js.Promise.then_(value => {
     let dogs = value##data##dogs;
     Js.log(dogs);
     Js.Promise.resolve(dogs);
   })
|> Js.Promise.catch(err => {
     Js.log2("Something went wrong!", err);
     Js.Promise.resolve(err);
   });

Client.executeQuery(~client, ~query=queryOneDog, ~skipCache=false)
|> Js.Promise.then_(value => {
     let dog = value##data##dog;
     Js.log(dog);
     Js.Promise.resolve(dog);
   })
|> Js.Promise.catch(err => {
     Js.log2("Something went wrong!", err);
     Js.Promise.resolve(err);
   });

/* Cool, let's render the App now! */
ReactDOMRe.renderToElementWithId(<App client />, "root");