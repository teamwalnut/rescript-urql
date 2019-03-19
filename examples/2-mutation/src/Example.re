open ReasonUrql;

type dogs = {
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
  query dogs {
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

let parse = GetAllDogs.make()##parse;

let queryRequest = Request.createRequest(~query=GetAllDogs.make()##query, ());

let mutationLikeDog = {|
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      likes
    }
  }
|};

module LikeDog = [%graphql
  {|
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      likes
    }
  }
|}
];

let mutationPatDog = {|
  mutation patDog($key: ID!) {
    patDog(key: $key) {
      pats
    }
  }
|};

module PatDog = [%graphql
  {|
  mutation patDog($key: ID!) {
    patDog(key: $key) {
      pats
    }
  }
  |}
];

let mutationTreatDog = {|
  mutation treatDog($key: ID!) {
    treatDog(key: $key) {
      treats
    }
  }
|};
module TreatDog = [%graphql
  {|
  mutation treatDog($key: ID!) {
    treatDog(key: $key) {
      treats
    }
  }
  |}
];

let mutationBellyscratchDog = {|
  mutation bellyscratchDog($key: ID!) {
    bellyscratchDog(key: $key) {
      bellyscratches
    }
  }
|};
module BellyscratchDog = [%graphql
  {|
  mutation bellyscratchDog($key: ID!) {
    bellyscratchDog(key: $key) {
      bellyscratches
    }
  }
  |}
];

module Styles = {
  open Css;

  let container =
    style([
      position(absolute),
      display(grid),
      gridTemplateRows([`fr(1.), `fr(1.), `fr(1.)]),
      gridTemplateColumns([`fr(1.), `fr(1.), `fr(1.), `fr(1.)]),
      height(pct(100.)),
      width(pct(100.)),
    ]);

  let button = shade =>
    style([
      alignSelf(`center),
      border(px(3), `solid, hex(shade)),
      color(hex(shade)),
      fontSize(rem(1.25)),
      margin(px(10)),
      cursor(`pointer),
      borderRadius(px(10)),
      hover([backgroundColor(hex(shade)), color(white)]),
    ]);

  let emoji = style([fontSize(rem(2.))]);

  let fadeInOut = shade =>
    keyframes([
      (0, [backgroundColor(white)]),
      (50, [backgroundColor(hex(shade))]),
      (100, [backgroundColor(white)]),
    ]);

  let highlight = shade =>
    style([animationName(shade->fadeInOut), animationDuration(1000)]);
};

type highlight = (string, string);

type state = {
  dogs: array(dogs),
  highlight,
};

type action =
  | ExecuteQuery
  | SetQuery(array(dogs))
  | HighlightDog(highlight)
  | DehighlightDog;

let component = ReasonReact.reducerComponent("Example");

let make = (~client: Client.t, _children) => {
  ...component,
  initialState: () => {dogs: [||], highlight: ("", "")},
  reducer: (action, state) => {
    switch (action) {
    | ExecuteQuery =>
      ReasonReact.SideEffects(
        self =>
          Client.executeQuery(~client, ~query=queryRequest)
          |> Wonka.forEach((. response) => {
               let data = response##data->parse;
               switch (data##dogs) {
               | Some(data) =>
                 self.send(
                   SetQuery(data->Belt.Array.slice(~offset=0, ~len=8)),
                 )
               | None => self.send(SetQuery([||]))
               };
             }),
      )
    | SetQuery(dogs) => ReasonReact.Update({...state, dogs})
    | HighlightDog(highlight) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, highlight},
        self =>
          Js.Global.setTimeout(() => self.send(DehighlightDog), 1000)
          |> ignore,
      )
    | DehighlightDog => ReasonReact.Update({...state, highlight: ("", "")})
    };
  },
  didMount: self => self.send(ExecuteQuery),
  render: self => {
    let variables = Js.Dict.empty();
    let randomDog = Belt.Array.get(Belt.Array.shuffle(self.state.dogs), 0);
    let key =
      switch (randomDog) {
      | Some(dog) => dog.key
      | None => ""
      };
    Js.Dict.set(variables, "key", Js.Json.string(key));
    let payload = Js.Json.object_(variables);

    <div className=Styles.container>
      <Mutation query=mutationLikeDog>
        ...{({executeMutation}: Mutation.mutationRenderProps(LikeDog.t)) =>
          <button
            className={"48a9dc"->Styles.button}
            onClick={_event => {
              executeMutation(payload) |> ignore;
              self.send(HighlightDog((key, "48a9dc")));
            }}>
            "Give a Dog a Like!"->ReasonReact.string
            <br />
            <span className=Styles.emoji>
              {js|👍|js}->ReasonReact.string
            </span>
          </button>
        }
      </Mutation>
      <Mutation query=mutationPatDog>
        ...{({executeMutation}: Mutation.mutationRenderProps(PatDog.t)) =>
          <button
            className={"db4d3f"->Styles.button}
            onClick={_event => {
              executeMutation(payload) |> ignore;
              self.send(HighlightDog((key, "db4d3f")));
            }}>
            "Give a Dog a Pat!"->ReasonReact.string
            <br />
            <span className=Styles.emoji>
              {js|✋|js}->ReasonReact.string
            </span>
          </button>
        }
      </Mutation>
      <Mutation query=mutationTreatDog>
        ...{({executeMutation}: Mutation.mutationRenderProps(TreatDog.t)) =>
          <button
            className={"BD11E7"->Styles.button}
            onClick={_event => {
              executeMutation(payload) |> ignore;
              self.send(HighlightDog((key, "BD11E7")));
            }}>
            "Throw a Dog a Bone!"->ReasonReact.string
            <br />
            <span className=Styles.emoji>
              {js|🍖|js}->ReasonReact.string
            </span>
          </button>
        }
      </Mutation>
      <Mutation query=mutationBellyscratchDog>
        ...{(
          {executeMutation}: Mutation.mutationRenderProps(BellyscratchDog.t),
        ) =>
          <button
            className={"1BDA2A"->Styles.button}
            onClick={_event => {
              executeMutation(payload) |> ignore;
              self.send(HighlightDog((key, "1BDA2A")));
            }}>
            "Give a Dog a Bellyscratch!"->ReasonReact.string
            <br />
            <span className=Styles.emoji>
              {js|🐾|js}->ReasonReact.string
            </span>
          </button>
        }
      </Mutation>
      {Array.map(
         dog =>
           <Dog
             key={dog.key}
             name={dog.name}
             imageUrl={dog.imageUrl}
             likes={dog.likes}
             pats={dog.pats}
             treats={dog.treats}
             bellyscratches={dog.bellyscratches}
             className={
               dog.key === fst(self.state.highlight) ?
                 snd(self.state.highlight)->Styles.highlight : ""
             }
           />,
         self.state.dogs,
       )
       ->ReasonReact.array}
    </div>;
  },
};