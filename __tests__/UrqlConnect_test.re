open Jest;
open ReasonUrql;

Enzyme.configureEnzyme(Enzyme.react_16_adapter());

describe("Connect", () => {
  let testQuery = Query.query(TestUtils.TestQuery.make());
  let testMutation =
    Mutation.mutation(
      TestUtils.TestMutationWithVariable.make(~key="VmeRTX7j-", ()),
    );
  let testRender = (_result: Connect.renderArgs('a, 'b, 'c)) =>
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

  describe("urqlDataToVariant", () => {
    let urqlData = {
      "loaded": true,
      "data":
        Js.Nullable.return({
          "dogs": [|
            {"key": "VmeRTX7j-", "name": "Dixie", "breed": "Pit Bull"},
          |],
        }),
      "error": Js.Nullable.null,
      "cache": {
        "invalidateAll": () =>
          Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a)),
        "invalidate": (~query: option(UrqlQuery.urqlQuery)=?) =>
          Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a)),
        "update": TestUtils.update,
        "read": (~query as _: UrqlQuery.urqlQuery) =>
          Js.Promise.make((~resolve, ~reject as _) => resolve(. "data")),
      },
    };

    test(
      "should convert the supplied urql data to our variant constructors", () =>
      Expect.(
        expect(urqlData |> Connect.urqlDataToVariant)
        |> toEqual(
             Js.Obj.assign(
               urqlData,
               {"response": Connect.Data(urqlData##data)},
             ),
           )
      )
    );

    test("should convert a loading response to a Loading constructor", () => {
      let urqlData = Js.Obj.assign(urqlData, {"loaded": false});

      Expect.(
        expect(urqlData |> Connect.urqlDataToVariant)
        |> toEqual(Js.Obj.assign(urqlData, {"response": Connect.Loading}))
      );
    });

    test("should convert an error response to an Error constructor", () => {
      let error = Connect.error(~message="Failed to fetch.");
      let urqlData =
        Js.Obj.assign(urqlData, {"error": Js.Nullable.return(error)});

      Expect.(
        expect(urqlData |> Connect.urqlDataToVariant)
        |> toEqual(
             Js.Obj.assign(urqlData, {"response": Connect.Error(error)}),
           )
      );
    });

    test(
      "should return an error response if loaded and no data and no error are present",
      () => {
        let urqlData =
          Js.Obj.assign(
            urqlData,
            {"error": Js.Nullable.null, "data": Js.Nullable.null},
          );

        Expect.(
          expect(urqlData |> Connect.urqlDataToVariant)
          |> toEqual(
               Js.Obj.assign(
                 urqlData,
                 {"response": Connect.Error(Connect.noData)},
               ),
             )
        );
      },
    );
  });
});