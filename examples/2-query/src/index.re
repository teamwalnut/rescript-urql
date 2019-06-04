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

/* Creates our query we pass into Query component */
let query = request##query;

let variables = request##variables;

/* Flattens our pokemon list from [{name: "bob"}] -> ["bob"] */
let pokemonList = pokemons =>
  Js.Array.map(
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
    <Query query variables>

        ...{
             ({response}: Query.queryRenderProps(GetAllPokemons.t)) =>
               <main>
                 {
                   switch (response) {
                   | Data(data) =>
                     switch (data##pokemons) {
                     | Some(pokemons) =>
                       /* Provided all the data is right give
                          it all to the GetAll component */
                       let pokemons = pokemons->pokemonList;
                       <GetAll pokemons />;
                     | None => <div> "No Data"->ReasonReact.string </div>
                     }
                   | Fetching => <div> "Loading"->ReasonReact.string </div>
                   | Error(error) => <div> "Error!"->ReasonReact.string </div>
                   | NotFound => <div> "Not Found"->ReasonReact.string </div>
                   }
                 }
               </main>
           }
      </Query>
      /* Using render props we get the response off the Query component */
  </Provider>,
  "root",
);
