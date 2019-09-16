open ReasonUrql;
open Client;

type dog = {
  key: string,
  name: string,
  breed: string,
  likes: int,
  pats: int,
  treats: int,
  bellyscratches: int,
  description: string,
  imageUrl: string,
};

module GetAllDogs = [%graphql
  {|
  {
    dogs @bsRecord {
      key
      name
      breed
      likes
      pats
      treats
      bellyscratches
      description
      imageUrl
    }
  }
|}
];

let request = GetAllDogs.make();

type state = {dogs: array(dog)};

type action = {payload: state};

[@react.component]
let make = (~client: Client.t) => {
  let (state, dispatch) =
    React.useReducer(
      (_, action) => {dogs: action.payload.dogs},
      {dogs: [||]},
    );

  React.useEffect1(
    () => {
      let query =
        Client.executeQuery(~client, ~request, ())
        |> Wonka.subscribe((. data) =>
             switch (data.response) {
             | Data(d) => dispatch({
                            payload: {
                              dogs: d##dogs,
                            },
                          })
             | _ => ()
             }
           );

      Some(query.unsubscribe);
    },
    [|client|],
  );

  <div className=GridStyles.grid>
    {Array.map(
       dog =>
         <Dog
           key={dog.key}
           id={dog.key}
           name={dog.name}
           imageUrl={dog.imageUrl}
           likes={dog.likes}
           pats={dog.pats}
           treats={dog.treats}
           bellyscratches={dog.bellyscratches}
           description={dog.description}
         />,
       state.dogs,
     )
     ->React.array}
  </div>;
};
