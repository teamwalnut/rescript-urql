open ReasonUrql;

module GetAllDogs = [%graphql
  {|
  query dogs {
    dogs {
      name
      breed
      description
    }
  }
|}
];

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

let client = Client.make(~url="https://formidadog-ql.now.sh", ());

let queryRequest = Request.createRequest(~query=GetAllDogs.make()##query, ());

Client.executeQuery(~client, ~query=queryRequest)
|> Wonka.subscribe((. response) => Js.log2("query response", response));

let mutation = LikeDog.make(~key="VmeRTX7j-", ());
let mutationRequest =
  Request.createRequest(
    ~query=mutation##query,
    ~variables=mutation##variables,
    (),
  );

Client.executeMutation(~client, ~mutation=mutationRequest)
|> Wonka.subscribe((. response) => Js.log2("mutation response", response));