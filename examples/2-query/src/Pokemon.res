open ReasonUrql

module GetPokemon = %graphql(
  `
  query pokemon($name: String!) {
    pokemon(name: $name) {
      name
      classification
      height {
        maximum
      }
      weight {
        maximum
      }
      image
    }
  }
`
)

@react.component
let make = (~pokemon: string) => {
  let ({Hooks.response: response}, _) = Hooks.useQuery(~query=module(GetPokemon), {name: pokemon})

  switch response {
  | Fetching => <div> {"Loading"->React.string} </div>
  | Data(data)
  | PartialData(data, _) =>
    switch data.pokemon {
    | Some(pokemon) =>
      switch (pokemon.image, pokemon.classification, pokemon.name, pokemon.height, pokemon.weight) {
      | (Some(image), Some(classification), Some(name), Some(height), Some(weight)) =>
        <section className="dex-container">
          <div className="dex">
            <div className="dex__image-container"> <img className="dex__image" src=image /> </div>
            <div className="dex__text">
              <h1 className="dex__title"> {name->React.string} </h1>
              <h2 className="dex__subtitle"> {classification->React.string} </h2>
              {switch (height.maximum, weight.maximum) {
              | (Some(heightMax), Some(weightMax)) =>
                <div className="dex__stats">
                  <p> {("Height: " ++ heightMax)->React.string} </p>
                  <p> {("Weight: " ++ weightMax)->React.string} </p>
                </div>
              | (_, _) => React.null
              }}
            </div>
          </div>
        </section>
      | (_, _, _, _, _) => React.null
      }
    | None => React.null
    }
  | Error(e) => <div> {e.message->React.string} </div>
  | Empty => <div> {"Not Found"->React.string} </div>
  }
}
