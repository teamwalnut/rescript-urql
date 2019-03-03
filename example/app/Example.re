open ReasonUrql;

/* Setup a GraphQL query to get all dogs from our API. */
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
let queryAllDogs = Query.query(GetAllDogs.make());

/* Setup a GraphQL mutation to increment a dog's likes. */
module LikeDog = [%graphql
  {|
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      name
      key
      breed
      likes
    }
  }
|}
];

let likeDog = Mutation.mutation(LikeDog.make(~key="VmeRTX7j-", ()));

/* Setup some standard fetch options for the client. */
let fetchOptions =
  Client.FetchObj(
    Fetch.RequestInit.make(
      ~method_=Post,
      ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
      (),
    ),
  );

/* Set up our client. */
let client =
  Client.make(~url="https://formidadog-ql.now.sh", ~fetchOptions, ());

/* Let's execute a query and a mutation! */
Client.executeQuery(~client, ~query=queryAllDogs, ~skipCache=false)
|> Js.Promise.then_(value => {
     let dogs = value##data##dogs;
     Js.log2("Dogs", value##data##dogs);
     Js.Promise.resolve(dogs);
   })
|> Js.Promise.catch(err => {
     Js.log2("Something went wrong!", err);
     Js.Promise.resolve(err);
   });

Client.executeMutation(~client, ~mutation=likeDog)
|> Js.Promise.then_(value => {
     let likeDog = value##likeDog;
     Js.log2("Like Dog", likeDog);
     Js.Promise.resolve(likeDog);
   })
|> Js.Promise.catch(err => {
     Js.log2("Something went wrong!", err);
     Js.Promise.resolve(err);
   });

/* Cool, let's render the App now! */
ReactDOMRe.renderToElementWithId(<App />, "root");