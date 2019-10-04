/* Open our reason urql module */

open ReasonUrql;

/* Instatiate the client */
let client = Client.make(~url="https://graphql-pokemon.now.sh", ());

/* graphql query for our list of pokemon */
module GetAllPokemons = [%graphql
  {|
  query pokemons($first: Int!) {
    pokemons(first: $first) {
      name
    }
  }
|}
];

/* How many pokemon we want to get, in this case 151 */
let numberOfPokemon = 151;
let request = GetAllPokemons.make(~first=numberOfPokemon, ());

/* Flattens our pokemon list from [{name: "bob"}] -> ["bob"] */
let pokemonList = pokemons =>
  Array.map(
    pokemon =>
      switch (pokemon) {
      | Some(pokemon) =>
        switch (pokemon##name) {
        | Some(name) => name
        | None => ""
        }
      | None => ""
      },
    pokemons,
  );

ReactDOMRe.renderToElementWithId(
  <Provider value=client>
    <Query request>
      ...{({response}) =>
        <main>
          {switch (response) {
           | Data(data) =>
             switch (data##pokemons) {
             | Some(pokemons) =>
               /* Provided all the data is right give
                  it all to the GetAll component */
               let pokemons = pokemons->pokemonList;
               <GetAll pokemons />;
             | None => <div> "No Data"->React.string </div>
             }
           | Fetching => <div> "Loading"->React.string </div>
           | Error(e) =>
             switch (e) {
             | {networkError: Some(ne)} =>
               <div>
                 {ne
                  ->Js.Exn.message
                  ->Belt.Option.getWithDefault("Network error")
                  ->React.string}
               </div>
             | {graphQLErrors: Some(gqle)} =>
               <div>
                 {gqle
                  |> Array.to_list
                  |> List.map(e => {
                       let msg =
                         Belt.Option.getWithDefault(
                           Js.Nullable.toOption(CombinedError.messageGet(e)),
                           "GraphQL error",
                         );
                       "[GraphQLError: " ++ msg ++ "]";
                     })
                  |> String.concat(", ")
                  |> React.string}
               </div>
             | _ => <div> "Unknown error."->React.string </div>
             }
           | NotFound => <div> "Not Found"->React.string </div>
           }}
        </main>
      }
    </Query>
  </Provider>,
  /* Using render props we get the response off the Query component */
  "root",
);
