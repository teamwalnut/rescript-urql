open ReasonUrql;

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
  query dogs {
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

let parse = GetAllDogs.make()##parse;

let queryRequest = Request.createRequest(~query=GetAllDogs.make()##query, ());

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
      let subscription =
        Client.executeQuery(~client, ~query=queryRequest, ())
        |> Wonka.subscribe((. response) => {
             let dogs =
               switch (response##data->parse##dogs) {
               | Some(data) => data
               | None => [||]
               };
             dispatch({
               payload: {
                 dogs: dogs,
               },
             });
           });
      Some(subscription.unsubscribe);
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