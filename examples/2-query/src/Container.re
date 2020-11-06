open ReasonUrql;

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

let flattenPokemon = pokemons => {
  GetAllPokemons.(
    pokemons->Belt.Array.map(pokemon =>
      pokemon
      ->Belt.Option.flatMap(p => p.name)
      ->Belt.Option.getWithDefault("")
    )
  );
};

[@react.component]
let make = () => {
  let (Hooks.{response}, _) =
    Hooks.useQuery(
      ~query=(module GetAllPokemons),
      ~requestPolicy=`CacheFirst,
      GetAllPokemons.makeVariables(~first=151, ()),
    );

  switch (response) {
  | Fetching => <div> "Loading"->React.string </div>
  | Data(data) =>
    switch (data.pokemons) {
    | Some(pokemon) => <PokemonList pokemon={pokemon->flattenPokemon} />
    | None => <div> "No Data"->React.string </div>
    }
  | PartialData(data, e) =>
    <div>
      {switch (data.pokemons) {
       | Some(pokemon) => <PokemonList pokemon={pokemon->flattenPokemon} />
       | None => <div> "No Data"->React.string </div>
       }}
      <p>
        {e
         |> Array.to_list
         |> List.map((e: GraphQLError.t) => {
              "[GraphQLError: " ++ e.message ++ "]"
            })
         |> String.concat(", ")
         |> React.string}
      </p>
    </div>
  | Error(e) =>
    switch (e) {
    | {networkError: Some(ne)} =>
      <div>
        {ne
         ->Js.Exn.message
         ->Belt.Option.getWithDefault("Network error")
         ->React.string}
      </div>
    | _ => <div> "Unknown error."->React.string </div>
    }
  | Empty => <div> "Not Found"->React.string </div>
  };
};
