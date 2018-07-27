let component = ReasonReact.statelessComponent("DogList");

let query: ReasonUrql.Query.urqlQuery =
  ReasonUrql.Query.query(
    ~query=
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
 |},
    (),
  );

let likeDog: ReasonUrql.Mutation.urqlMutation =
  ReasonUrql.Mutation.mutation(
    ~query=
      {|
    mutation likeDog($key: ID!) {
      likeDog(key: $key) {
        name
        key
        breed
        likes
      }
    }
  |},
    (),
  );

let mutationMap: ReasonUrql.Connect.mutationMap = Js.Dict.empty();

Js.Dict.set(mutationMap, "likeDog", likeDog);

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
external likeDog :
  (ReasonUrql.Connect.renderArgs(dogs), {. "key": string}) => unit =
  "";

let make = _children => {
  ...component,
  render: _self =>
    <ReasonUrql.Connect
      query=(`Query(query))
      mutation=mutationMap
      renderProp=(
        (~result: ReasonUrql.Connect.renderArgs(dogs)) => {
          let loaded = result |. ReasonUrql.Connect.loaded;
          let data = result |. ReasonUrql.Connect.data;
          let error =
            ReasonUrql.Connect.convertJsErrorToReason(
              result |. ReasonUrql.Connect.error,
            );
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
              (
                ReasonReact.string(
                  "Error: " ++ (obj |. ReasonUrql.Connect.message),
                )
              )
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
                    dog => {
                      let key = dog |. key;
                      <Dog
                        key={j|$key|j}
                        description=(dog |. description)
                        id=key
                        imageUrl=(dog |. imageUrl)
                        name=(dog |. name)
                        likes=(dog |. likes)
                        onClick=(result |. likeDog)
                      />;
                    },
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