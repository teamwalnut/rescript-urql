type state = {
  search: string,
  selectedPokemon: option(string),
};

type action =
  | ChangeInput(string)
  | SelectPokemon(string);

/* Filters pokemon list by input. */
let filterPokemon = (input: string, pokemon: array(string)) =>
  pokemon
  |> Array.to_list
  |> List.filter(pokemon =>
       Js.String.includes(
         String.lowercase_ascii(input),
         String.lowercase_ascii(pokemon),
       )
     );

[@react.component]
let make = (~pokemon) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | ChangeInput(search) => {...state, search}
        | SelectPokemon(selectedPokemon) => {
            ...state,
            selectedPokemon: Some(selectedPokemon),
          }
        },
      {search: "", selectedPokemon: Some("Bulbasaur")},
    );

  <div className="sidebar-container">
    <section className="sidebar">
      <input
        className="sidebar__search"
        value={state.search}
        onChange={event =>
          dispatch(ChangeInput(event->ReactEvent.Form.target##value))
        }
        placeholder="Search for a Pokemon..."
      />
      <ul className="sidebar__list">
        {pokemon
         |> filterPokemon(state.search)
         |> List.map(pokemon =>
              <li key=pokemon className="sidebar__list-item">
                <button
                  className="sidebar__button"
                  onClick={_event => dispatch(SelectPokemon(pokemon))}>
                  pokemon->React.string
                </button>
              </li>
            )
         |> Array.of_list
         |> React.array}
      </ul>
    </section>
    {switch (state.selectedPokemon) {
     | Some(pokemon) => <Pokemon pokemon />
     | None => React.null
     }}
  </div>;
};
