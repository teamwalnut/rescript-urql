open ReasonUrql;

module GetAllDogs = [%graphql
  {|
  {
    dogs {
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

[@react.component]
let make = () => {
  let ({Hooks.data}, _) = UseQuery.useQuery(~query=(module GetAllDogs), ());

  <div className="grid">
    {data
     ->Belt.Option.mapWithDefault([||], d => d.dogs)
     ->Belt.Array.map(dog =>
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
     ->React.array}
  </div>;
};
