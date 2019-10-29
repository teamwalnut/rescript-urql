open PokemonListStyles;

type state = {
  listOfPokemons: array(string),
  textInput: string,
  filteredList: list(string),
  selectedPokemon: option(string),
};

type action =
  | ChangeInput(string)
  | SelectPokemon(string);

/* Filters pokemon list by input. */
let filterPokemonList = (~pokemonArray: array(string), ~input: string) =>
  pokemonArray
  |> Array.to_list
  |> List.filter(pokemon =>
       Js.String.includes(
         String.lowercase(input),
         String.lowercase(pokemon),
       )
     );

[@react.component]
let make = (~pokemons) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | ChangeInput(textInput) => {
            ...state,
            textInput,
            filteredList:
              filterPokemonList(
                ~pokemonArray=state.listOfPokemons,
                ~input=textInput,
              ),
          }
        | SelectPokemon(selectedPokemon) => {
            ...state,
            selectedPokemon: Some(selectedPokemon),
          }
        },
      {
        listOfPokemons: pokemons,
        textInput: "",
        filteredList: filterPokemonList(~pokemonArray=pokemons, ~input=""),
        selectedPokemon: Some("Bulbasaur"),
      },
    );

  let buildPokemonList =
    React.useCallback0((pokemonList: list(string)) =>
      pokemonList
      |> List.map(pokemon =>
           <li key=pokemon className=Styles.listItem>
             <button
               className=Styles.button
               onClick={_event => dispatch(SelectPokemon(pokemon))}>
               pokemon->React.string
             </button>
           </li>
         )
    );

  <div className=Styles.container>
    <section className=Styles.aside>
      <input
        className=Styles.search
        value={state.textInput}
        onChange={event =>
          dispatch(ChangeInput(event->ReactEvent.Form.target##value))
        }
        placeholder="Search for a Pokemon..."
      />
      <ul className=Styles.list>
        {buildPokemonList(state.filteredList)->Array.of_list->React.array}
      </ul>
    </section>
    {switch (state.selectedPokemon) {
     | Some(pokemon) => <Pokemon pokemon />
     | None => React.null
     }}
  </div>;
};
