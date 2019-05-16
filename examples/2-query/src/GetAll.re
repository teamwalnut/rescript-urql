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

let component = ReasonReact.reducerComponent("GetAll");

let make = (~pokemons, _children) => {
  ...component,
  initialState: () => {
    listOfPokemons: pokemons,
    textInput: "",
    filteredList: filteredPokemonList(~pokemonList=pokemons, ~input=""),
    selectedPokemon: None,
  },
  reducer: (action, state) =>
    switch (action) {
    | ChangeInput(textInput) =>
      ReasonReact.Update({
        ...state,
        textInput,
        filteredList:
          filteredPokemonList(
            ~pokemonList=state.listOfPokemons,
            ~input=textInput,
          ),
      })
    | SelectPokemon(selected) =>
      ReasonReact.Update({...state, selectedPokemon: Some(selected)})
    },
  render: self => {
    let pokemonElementsArray = (~pokemonList: array(string)) =>
      Array.map(
        pokemon =>
          <li key=pokemon className=Styles.list>
            <button
              className=Styles.listButton
              onClick={_event => self.send(SelectPokemon(pokemon))}>
              pokemon->ReasonReact.string
            </button>
          </li>,
        pokemonList,
      );
    <div className=Styles.container>
      <section className=Styles.search>
        <input
          className=Styles.searchBox
          value={self.state.textInput}
          onChange={
            event =>
              self.send(ChangeInput(event->ReactEvent.Form.target##value))
          }
        />
        <ul>
          {pokemonElementsArray(~pokemonList=self.state.filteredList)
          ->ReasonReact.array}
        </ul>
      </section>
      {
        switch (self.state.selectedPokemon) {
        | Some(pokemon) => <Monster pokemon />
        | None => ReasonReact.null
        }
      }
    </div>;
  },
};