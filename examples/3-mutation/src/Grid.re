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

[@react.component]
let make = () => {
  let (Hooks.{response}, _) = Hooks.useQuery(~request, ());

  <div className="grid">
    {switch (response) {
     | Fetching => <div> "Loading"->React.string </div>
     | Data(d)
     | PartialData(d, _) =>
       d##dogs
       |> Array.map(dog =>
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
            />
          )
       |> React.array
     | Error(_e) => <div> "Error"->React.string </div>
     | Empty => <div> "Empty"->React.string </div>
     }}
  </div>;
};
