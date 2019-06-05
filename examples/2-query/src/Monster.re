open MonsterStyles;
open ReasonUrql;
open Hooks;

type maximum = {maximum: int};

type pokemon = {
  id: string,
  number: string,
  height: maximum,
  weight: maximum,
};

/* graphql query for GetPokemon */

module GetPokemon = [%graphql
  {|
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
|}
];

[@react.component]
let make = (~pokemon: string) => {
  /* We set up the query here as we need access to the pokemon
      value passed in from GetAll */
  let request = GetPokemon.make(~name=pokemon, ());
  let query = request##query;
  let variables = request##variables;
  let ({response}, _executeQuery) = useQuery(~query, ~variables, ());

  <>
    {
      switch (response) {
      | Data(data) =>
        switch (data##pokemon) {
        | Some(pokemon) =>
          switch (
            pokemon##image,
            pokemon##classification,
            pokemon##name,
            pokemon##height,
            pokemon##weight,
          ) {
          | (
              Some(image),
              Some(classification),
              Some(name),
              Some(height),
              Some(weight),
            ) =>
            <section className=Styles.dexContainer>
              <div className=Styles.dex>
                <div className=Styles.dexImageContainer>
                  <img className=Styles.dexImage src=image />
                </div>
                <div className=Styles.dexText>
                  <h1 className=Styles.dexTitle>
                    name->ReasonReact.string
                  </h1>
                  <h2 className=Styles.dexSubTitle>
                    classification->ReasonReact.string
                  </h2>
                  {
                    switch (height##maximum, weight##maximum) {
                    | (Some(heightMax), Some(weightMax)) =>
                      <div className=Styles.dexGrid>
                        <p>
                          ("Height: " ++ heightMax)
                          ->ReasonReact.string
                        </p>
                        <p>
                          ("Weight: " ++ weightMax)
                          ->ReasonReact.string
                        </p>
                      </div>
                    | (_, _) => ReasonReact.null
                    }
                  }
                </div>
              </div>
            </section>
          | (_, _, _, _, _) => ReasonReact.null
          }
        | None => ReasonReact.null
        }
      | Fetching => <div> "Loading"->ReasonReact.string </div>
      | Error(error) => {
        Js.log(error);
        <div> "Error"->ReasonReact.string </div>
      }
      | NotFound => <div> "Not Found"->ReasonReact.string </div>
      }
    }
  </>
};
