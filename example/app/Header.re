open ReasonUrql;

[@bs.module] external formidableLogo: string = "./assets/formidable-logo.svg";

let component = ReasonReact.statelessComponent("Header");

module LikeAllDogs = [%graphql
  {|
  mutation likeAllDogs {
    likeAllDogs {
      key
      likes
    }
  }
|}
];

let likeAllDogs = Mutation.mutation(LikeAllDogs.make());

let mutationMap: Connect.mutationMap = Js.Dict.empty();
Js.Dict.set(mutationMap, "likeAllDogs", likeAllDogs);

[@bs.send]
external likeAllDogs:
  (Connect.renderArgs('data, 'store, 'value), unit) => unit =
  "";

let make = _children => {
  ...component,
  render: _self =>
    <Connect
      mutation=mutationMap
      render={
        result =>
          <header
            style={
              ReactDOMRe.Style.make(
                ~minHeight="15%",
                ~background="#a1a1a1",
                ~padding="10px",
                ~display="flex",
                ~alignItems="center",
                ~fontFamily="Space Mono",
                (),
              )
            }>
            <img
              src=formidableLogo
              onClick={_e => result->(likeAllDogs())}
              style={ReactDOMRe.Style.make(~cursor="pointer", ())}
            />
            <span
              style={
                ReactDOMRe.Style.make(
                  ~fontSize="25px",
                  ~marginLeft="10px",
                  (),
                )
              }>
              {ReasonReact.string("Dogs")}
            </span>
          </header>
      }
    />,
};