open ReasonUrql;

let component = ReasonReact.statelessComponent("DogList");

module AllDogsQuery = [%graphql
  {|
  query {
    dogs {
      name
      key
      breed
      description
      imageUrl
      likes
    }
  }
|}
];

module GetDogs = Query.CreateQuery(AllDogsQuery);

module DogMutation = [%graphql
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

module LikeDog = Mutation.CreateMutation(DogMutation);

let mutationMap: Connect.mutationMap = Js.Dict.empty();
LikeDog.addMutationToMap(~dict=mutationMap, ~key="likeDog");

[@bs.deriving abstract]
type dog = {
  name: string,
  key: string,
  description: string,
  likes: int,
  imageUrl: string,
};

[@bs.deriving abstract]
type dogs = {dogs: array(dog)};

[@bs.send]
external likeDog : (Connect.renderArgs(dogs), {. "key": string}) => unit =
  "";

let make = _children => {
  ...component,
  render: _self =>
    <Connect
      query=(`Query(GetDogs.query))
      mutation=mutationMap
      renderProp=(
        (~result: Connect.renderArgs(dogs)) => {
          let loaded = result |. Connect.loaded;
          let data = result |. Connect.data;
          let error = Connect.convertJsErrorToReason(result |. Connect.error);
          switch (error) {
          | Some(obj) =>
            <div
              style=(
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
              )>
              (ReasonReact.string("Error: " ++ (obj |. Connect.message)))
            </div>
          | None =>
            switch (loaded) {
            | false => <div> (ReasonReact.string("Loading")) </div>
            | true =>
              <div
                style=(
                  ReactDOMRe.Style.make(
                    ~display="grid",
                    ~gridTemplateColumns="1fr 1fr 1fr",
                    (),
                  )
                )>
                (
                  Array.map(
                    dog =>
                      <Dog
                        key=(dog |. key)
                        description=(dog |. description)
                        id=(dog |. key)
                        imageUrl=(dog |. imageUrl)
                        name=(dog |. name)
                        likes=(dog |. likes)
                        onClick=(result |. likeDog)
                      />,
                    data |. dogs,
                  )
                  |> ReasonReact.array
                )
              </div>
            }
          };
        }
      )
    />,
};