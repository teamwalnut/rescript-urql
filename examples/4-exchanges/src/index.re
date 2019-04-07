open ReasonUrql;

let client =
  Client.make(
    ~url="https://formidadog-ql.now.sh",
    ~exchanges=[|Exchanges.debugExchange, Exchanges.fetchExchange|],
    (),
  );

module GetAllDogs = [%graphql
  {|
  query dogs {
    dogs {
      name
      breed
      likes
    }
  }
|}
];

let queryRequest = Request.createRequest(~query=GetAllDogs.make()##query, ());

Client.executeQuery(~client, ~query=queryRequest)
|> Wonka.subscribe((. response) => ());