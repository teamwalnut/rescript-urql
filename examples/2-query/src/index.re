open ReasonUrql;

/* Instatiate the client. */
let client = Client.make(~url="https://graphql-pokemon.now.sh", ());

/* Fetch all 151 original pokemon. */
module GetAllPokemons = [%graphql
  {|
  query pokemons($first: Int!) {
    pokemons(first: $first) {
      name
    }
  }
|}
];

let request = GetAllPokemons.make(~first=151, ());

let flattenPokemon = pokemons =>
  pokemons
  |> Array.map(pokemon =>
       switch (pokemon) {
       | Some(pokemon) => Belt.Option.getWithDefault(pokemon##name, "")
       | None => ""
       }
     );

ReactDOMRe.renderToElementWithId(
  <Provider value=client>
    <Query request>
      ...{({response}) =>
        switch (response) {
        | Data(data) =>
          switch (data##pokemons) {
          | Some(pokemons) =>
            <PokemonList pokemons={pokemons->flattenPokemon} />
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
        }
      }
    </Query>
  </Provider>,
  "root",
);
