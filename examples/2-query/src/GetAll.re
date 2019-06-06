/* Opens our styles module */
open GetAllStyles;

type state = {
  listOfPokemons: array(Js.String.t),
  textInput: string,
  filteredList: array(Js.String.t),
  selectedPokemon: option(string),
};

type action =
  | ChangeInput(string)
  | SelectPokemon(string);

/* Filters our pokemon list by our input */
let filteredPokemonList = (~pokemonList: array(Js.String.t), ~input: string) =>
  pokemonList
  |> Array.to_list
  |> List.filter(pokemon =>
       Js.String.includes(
         String.lowercase(input),
         Js.String.toLowerCase(pokemon),
       )
     )
  |> Array.of_list;

[@react.component]
let make = (~pokemons) => {
  let (state, dispatch) = React.useReducer(
    (state, action) =>
    switch (action) {
    | ChangeInput(textInput) =>
      {
        ...state,
        textInput,
        filteredList:
          filteredPokemonList(
            ~pokemonList=state.listOfPokemons,
            ~input=textInput,
          ),
      }
    | SelectPokemon(selected) => {...state, selectedPokemon: Some(selected)}
    },
    {
      listOfPokemons: pokemons,
      textInput: "",
      filteredList: filteredPokemonList(~pokemonList=pokemons, ~input=""),
      selectedPokemon: None,
    }
  );

  let pokemonElementsArray = (~pokemonList: array(string)) =>
    Array.map(
      pokemon =>
        <li key=pokemon className=Styles.list>
          <button
            className=Styles.listButton
            onClick={_event => dispatch(SelectPokemon(pokemon))}>
            pokemon->React.string
          </button>
        </li>,
      pokemonList,
    );
  <div className=Styles.container>
    <section className=Styles.search>
      <input
        className=Styles.searchBox
        value={state.textInput}
        onChange={
          event =>
            dispatch(ChangeInput(event->ReactEvent.Form.target##value))
        }
      />
      <ul>
        {pokemonElementsArray(~pokemonList=state.filteredList)
        ->React.array}
      </ul>
    </section>
    {
      switch (state.selectedPokemon) {
      | Some(pokemon) => <Monster pokemon />
      | None => React.null
      }
    }
  </div>;
};
