open ReasonUrql

module GetAllDogs = %graphql(
  `
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
`
)

@react.component
let make = () => {
  let ({Hooks.response: response}, _) = UseQuery.useQuery(~query=module(GetAllDogs), ())

  <div className="grid">
    {switch response {
    | Fetching => <div> {"Loading"->React.string} </div>
    | Data(data)
    | PartialData(data, _) =>
      data.dogs
      ->Belt.Array.map(dog =>
        <Dog
          key=dog.key
          id=dog.key
          name=dog.name
          imageUrl=dog.imageUrl
          likes=dog.likes
          pats=dog.pats
          treats=dog.treats
          bellyscratches=dog.bellyscratches
          description=dog.description
        />
      )
      ->React.array
    | Error(e) => <div> {e.message->React.string} </div>
    | Empty => <div> {"Not Found"->React.string} </div>
    }}
  </div>
}
