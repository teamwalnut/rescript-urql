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

  let testQuery = Query.query(TestQuery.make());
  let testRender = (result: Connect.renderArgs('a, 'b, 'c)) => <div />;
  let setup = (~query=testQuery, ~render=testRender, ()) =>
    Enzyme.shallow(<Connect render query />);

  test("should render", () => {
    let wrapper = setup();
    Expect.(expect(wrapper) |> toMatchSnapshot);
  });
});