open ReasonUrql;

let component = ReasonReact.statelessComponent("MutationExample");

let mutation = {|
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      name
      breed
      likes
    }
  }
|};

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

let make = _children => {
  ...component,
  render: _self => {
    <Mutation query=mutation>
      ...{({ response }: Mutation.mutationRenderProps(LikeDog.t)) => {
        switch (response) {
          | Fetching => <div> "Fetching" -> ReasonReact.string </div>
          | Data(data) => <div> "Data" -> ReasonReact.string </div>
          | Error(error) => <div> "Error" -> ReasonReact.string </div>
          | NotFound => <div>"Not Found" -> ReasonReact.string </div>
        }
      }}
    </Mutation>;
  }
};