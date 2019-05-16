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

  let wrapper = style([display(flexBox), padding(px(20))]);

  let side =
    style([
      display(flexBox),
      flexDirection(column),
      alignItems(center),
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
      borderRadius(px(15)),
      padding(px(10)),
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
      width(vw(30.)),
    ]);

  type colors = {
    query: string,
    mutation: string,
  };

  let colors = {query: "48a9dc", mutation: "db4d3f"};
};

let str = ReasonReact.string;

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

let component = ReasonReact.reducerComponent("Previewer");

let make = _children => {
  ...component,
  initialState: () => {query: "", mutation: ""},
  reducer: (action, state) => {
    switch (action) {
    | ExecuteQuery =>
      ReasonReact.SideEffects(
        self =>
          Client.executeQuery(~client, ~query=queryRequest, ())
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
          Client.executeMutation(~client, ~mutation=mutationRequest, ())
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
    <div className=Styles.wrapper>
      <div className=Styles.side>
        <section className=Styles.section>
          <span className=Styles.title> "Query"->str </span>
          <div className={Styles.colors.query->Styles.code}>
            {|query dogs {
  dogs {
    name
    breed
    likes
  }
}|}->str
          </div>
          <button
            className={Styles.colors.query->Styles.button}
            onClick={_event => self.send(ExecuteQuery)}>
            "Execute Query"->str
          </button>
        </section>
        {switch (String.length(self.state.query)) {
         | 0 => ReasonReact.null
         | _ =>
           <section className=Styles.section>
             <span className=Styles.title> "Result"->str </span>
             <div className={Styles.colors.query->Styles.code}>
               self.state.query->str
             </div>
             <button
               className={Styles.colors.query->Styles.button}
               onClick={_event => self.send(ClearQuery)}>
               "Clear Query"->str
             </button>
           </section>
         }}
      </div>
      <div className=Styles.side>
        <section className=Styles.section>
          <span className=Styles.title> "Mutation"->str </span>
          <div className={"db4d3f"->Styles.code}>
            {|mutation likeDog($key: ID!) {
  likeDog(key: $key) {
    name
    breed
    likes
  }
}|}
            ->str
          </div>
          <button
            className={"db4d3f"->Styles.button}
            onClick={_event => self.send(ExecuteMutation)}>
            "Execute Mutation"->str
          </button>
        </section>
        {switch (String.length(self.state.mutation)) {
         | 0 => ReasonReact.null
         | _ =>
           <section className=Styles.section>
             <span className=Styles.title> "Result"->str </span>
             <div className={"db4d3f"->Styles.code}>
               self.state.mutation->str
             </div>
             <button
               className={"db4d3f"->Styles.button}
               onClick={_event => self.send(ClearMutation)}>
               "Clear Mutation"->str
             </button>
           </section>
         }}
      </div>
    </div>;
  },
};