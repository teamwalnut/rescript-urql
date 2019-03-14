open ReasonUrql;

let client = Client.make(~url="https://formidadog-ql.now.sh", ());

module GetAllDogs = [%graphql
  {|
  query dogs {
    dogs {
      name
      breed
      likes
    }
  }
|}
];

let queryRequest = Request.createRequest(~query=GetAllDogs.make()##query, ());

module LikeDog = [%graphql
  {|
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      name
      breed
      likes
    }
  }
|}
];

let mutation = LikeDog.make(~key="VmeRTX7j-", ());
let mutationRequest =
  Request.createRequest(
    ~query=mutation##query,
    ~variables=mutation##variables,
    (),
  );

module Styles = {
  open Css;

  let page =
    style([
      display(flexBox),
      top(px(0)),
      left(px(0)),
      bottom(px(0)),
      right(px(0)),
      position(absolute),
    ]);

  let side =
    style([
      display(flexBox),
      flexDirection(column),
      alignItems(center),
      justifyContent(center),
      flexBasis(pct(50.)),
    ]);

  let section = style([margin(px(10))]);

  let title =
    style([
      fontSize(rem(1.5)),
      fontFamily("Space Mono, 'sans-serif'"),
      margin(px(5)),
    ]);

  let button = bdColor =>
    style([
      borderColor(hex(bdColor)),
      color(hex(bdColor)),
      backgroundColor(white),
      fontSize(rem(1.2)),
      margin(px(10)),
      borderRadius(px(10)),
      padding(px(5)),
      cursor(`pointer),
    ]);

  let code = bgColor =>
    style([
      backgroundColor(hex(bgColor)),
      color(white),
      padding(px(20)),
      opacity(0.5),
      whiteSpace(`pre),
      fontFamily("Space Mono, 'sans-serif'"),
      borderRadius(px(5)),
      maxHeight(px(200)),
      overflow(`auto),
      minWidth(px(300)),
    ]);
};

type state = {
  query: string,
  mutation: string,
};

type action =
  | ExecuteQuery
  | SetQuery(string)
  | ClearQuery
  | ExecuteMutation
  | SetMutation(string)
  | ClearMutation;

let component = ReasonReact.reducerComponent("Example");

let make = _children => {
  ...component,
  initialState: () => {query: "", mutation: ""},
  reducer: (action, state) => {
    switch (action) {
    | ExecuteQuery =>
      ReasonReact.SideEffects(
        self =>
          Client.executeQuery(~client, ~query=queryRequest)
          |> Wonka.forEach((. response) =>
               self.send(
                 SetQuery(Js.Json.stringifyWithSpace(response##data, 2)),
               )
             ),
      )
    | SetQuery(query) => ReasonReact.Update({...state, query})
    | ClearQuery => ReasonReact.Update({...state, query: ""})
    | ExecuteMutation =>
      ReasonReact.SideEffects(
        self =>
          Client.executeMutation(~client, ~mutation=mutationRequest)
          |> Wonka.forEach((. response) =>
               self.send(
                 SetMutation(Js.Json.stringifyWithSpace(response##data, 2)),
               )
             ),
      )
    | SetMutation(mutation) => ReasonReact.Update({...state, mutation})
    | ClearMutation => ReasonReact.Update({...state, mutation: ""})
    };
  },
  render: self => {
    <div className=Styles.page>
      <div className=Styles.side>
        <section className=Styles.section>
          <div className=Styles.title> "Query"->ReasonReact.string </div>
          <div className={"48a9dc"->Styles.code}>
            {|query dogs {
  dogs {
    name
    breed
    likes
  }
}|}
            ->ReasonReact.string
          </div>
          <button
            className={"48a9dc"->Styles.button}
            onClick={_event => self.send(ExecuteQuery)}>
            "Execute Query"->ReasonReact.string
          </button>
        </section>
        {switch (String.length(self.state.query)) {
         | 0 => ReasonReact.null
         | _ =>
           <section className=Styles.section>
             <div className=Styles.title> "Result"->ReasonReact.string </div>
             <div className={"48a9dc"->Styles.code}>
               self.state.query->ReasonReact.string
             </div>
             <button
               className={"48a9dc"->Styles.button}
               onClick={_event => self.send(ClearQuery)}>
               "Clear Query"->ReasonReact.string
             </button>
           </section>
         }}
      </div>
      <div className=Styles.side>
        <section className=Styles.section>
          <div className=Styles.title> "Mutation"->ReasonReact.string </div>
          <div className={"db4d3f"->Styles.code}>
            {|mutation likeDog($key: ID!) {
  likeDog(key: $key) {
    name
    breed
    likes
  }
}|}
            ->ReasonReact.string
          </div>
          <button
            className={"db4d3f"->Styles.button}
            onClick={_event => self.send(ExecuteMutation)}>
            "Execute Mutation"->ReasonReact.string
          </button>
        </section>
        {switch (String.length(self.state.mutation)) {
         | 0 => ReasonReact.null
         | _ =>
           <section className=Styles.section>
             <span className=Styles.title> "Result"->ReasonReact.string </span>
             <div className={"db4d3f"->Styles.code}>
               self.state.mutation->ReasonReact.string
             </div>
             <button
               className={"db4d3f"->Styles.button}
               onClick={_event => self.send(ClearMutation)}>
               "Clear Mutation"->ReasonReact.string
             </button>
           </section>
         }}
      </div>
    </div>;
  },
};