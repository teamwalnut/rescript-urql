open ReasonUrql;

let component = ReasonReact.statelessComponent("Header");

let likeAllDogs: Mutation.urqlMutation =
  Mutation.mutation(
    ~query=
      {|
    mutation likeAllDogs {
      likeAllDogs {
        key
        likes
      }
    }
  |},
    (),
  );

let mutationMap: Connect.mutationMap = Js.Dict.empty();

Js.Dict.set(mutationMap, "likeAllDogs", likeAllDogs);

[@bs.send] external likeAllDogs : (Connect.renderArgs({.}), unit) => unit = "";

let make = _children => {
  ...component,
  render: _self =>
    <Connect
      mutation=mutationMap
      renderProp=(
        (~result: Connect.renderArgs({.})) =>
          <header
            style=(
              ReactDOMRe.Style.make(
                ~minHeight="15%",
                ~background="#a1a1a1",
                ~padding="10px",
                ~display="flex",
                ~alignItems="center",
                ~fontFamily="Space Mono",
                (),
              )
            )>
            <img
              src="../../assets/formidable-logo.svg"
              onClick=(_e => result |. likeAllDogs())
            />
            <span
              style=(
                ReactDOMRe.Style.make(
                  ~fontSize="25px",
                  ~marginLeft="10px",
                  (),
                )
              )>
              (ReasonReact.string("Dogs"))
            </span>
          </header>
      )
    />,
};