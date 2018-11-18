open Jest;
open ReasonUrql;

Enzyme.configureEnzyme(Enzyme.react_16_adapter());

describe("Connect", () => {
  module TestQuery = [%graphql
    {|
      query dogs {
        dogs {
          name
          breed
          description
        }
      }|}
  ];

  module TestMutation = [%graphql
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

  let testQuery = Query.query(TestQuery.make());
  let testMutation =
    Mutation.mutation(TestMutation.make(~key="VmeRTX7j-", ()));
  let testRender = (result: Connect.renderArgs('a, 'b, 'c)) =>
    <div> {ReasonReact.string("reason-urql rules!")} </div>;

  test("should render given a query and a function for the render prop", () => {
    let wrapper =
      Enzyme.shallow(<Connect render=testRender query=testQuery />);
    Expect.(expect(wrapper) |> toMatchSnapshot);
  });

  test(
    "should render given a mutation map and a function for the render prop", () => {
    let mutationMap = Js.Dict.empty();
    Js.Dict.set(mutationMap, "testMutation", testMutation);
    let wrapper =
      Enzyme.shallow(
        <Connect render=testRender query=testQuery mutation=mutationMap />,
      );

    Expect.(expect(wrapper) |> toMatchSnapshot);
  });
});