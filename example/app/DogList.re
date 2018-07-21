let component = ReasonReact.statelessComponent("DogList");

let query: Query.urqlQuery =
   Query.query(
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

let likeDog: Mutation.urqlMutation = Mutation.mutation(
  ~query={|
    mutation likeDog($key: ID!) {
      likeDog(key: $key) {
        name
        key
        breed
        likes
      }
    }
  |}, ()
);

let mutationMap: Connect.mutationMap = Js.Dict.empty();
Js.Dict.set(mutationMap, "likeDog", likeDog);

[@bs.deriving abstract]
type dog = {
  name: string,
  key: string,
  description: string,
  likes: int,
  imageUrl: string
};

[@bs.deriving abstract]
type dogs = {
  dogs: array(dog)
};

[@bs.send] external likeDog : Connect.renderArgs(dogs) => {. "key": string} => unit = "";

let make = (_children) => {
  ...component,
  render: _ => <Connect
    query={`Query(query)}
    mutation={mutationMap}
    renderProp={(~result: Connect.renderArgs(dogs)) => {
      let loaded = result |. Connect.loaded;
      let data = result |. Connect.data;
      let error = result |. Connect.error |> Js.Nullable.toOption;

      switch (error) {
      | Some(obj) => <div>(ReasonReact.string(obj |. Connect.message))</div>
      | None => {
        switch (loaded) {
          | false => <div>(ReasonReact.string("Loading"))</div>
          | true => <div style=(
                ReactDOMRe.Style.make(~display="grid", ~gridTemplateColumns="1fr 1fr 1fr", ())
              )>(
                Array.map((dog) => {
                  let key = dog |. key;
                  <Dog
                    key={j|$key|j}
                    description={dog |. description}
                    id={key}
                    imageUrl={dog |. imageUrl}
                    name={dog |. name}
                    likes={dog |. likes}
                    onClick={result |. likeDog}
                  />
                }, data |. dogs)
                |> ReasonReact.array
              )</div>
          } 
        }
      }
    }
  }/>
};
