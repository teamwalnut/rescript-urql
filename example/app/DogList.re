open ReasonUrql;

let component = ReasonReact.statelessComponent("DogList");

module GetAllDogs = [%graphql
  {|
query dogs {
  dogs {
    key
    name
    breed
    description
    imageUrl
    likes
  }
}|}
];

let query = Query.query(GetAllDogs.make());

module LikeDog = [%graphql
  {|
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      name
      key
      breed
      likes
    }
  }
|}
];

module LikeDogMutation = Mutation.Make(LikeDog);

let mutationMap: Connect.mutationMap = Js.Dict.empty();
Js.Dict.set(mutationMap, "likeDog", LikeDogMutation.mutation);

[@bs.send]
external likeDog:
  (Connect.renderArgs(GetAllDogs.t), {. "key": string}) => unit =
  "";

let make = _children => {
  ...component,
  render: _self =>
    <Connect
      query={`Query(query)}
      mutation=mutationMap
      render={
        (result: Connect.renderArgs(GetAllDogs.t)) =>
          switch (result##response) {
          | Loading => <div> {ReasonReact.string("Loading")} </div>
          | Data(data) =>
            <div
              style={
                ReactDOMRe.Style.make(
                  ~display="grid",
                  ~gridTemplateColumns="1fr 1fr 1fr",
                  (),
                )
              }>
              {
                Array.map(
                  dog =>
                    switch (dog) {
                    | Some(dog) =>
                      <Dog
                        key=dog##key
                        description=dog##description
                        id=dog##key
                        imageUrl=dog##imageUrl
                        name=dog##name
                        likes=dog##likes
                        onClick=result->likeDog
                      />
                    | None => <div />
                    },
                  data##dogs,
                )
                |> ReasonReact.array
              }
            </div>
          | Error(error) =>
            <div
              style={
                ReactDOMRe.Style.make(
                  ~display="flex",
                  ~padding="20px",
                  ~background="#f37fbf",
                  ~color="#900e56",
                  ~margin="20px",
                  ~fontFamily="Space Mono",
                  ~fontSize="20px",
                  (),
                )
              }>
              {ReasonReact.string("Error: " ++ error##message)}
            </div>
          }
      }
    />,
};