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

let str = React.string;

type state = {
  query: string,
  mutation: string,
};

type action =
  | SetQuery(string)
  | ClearQuery
  | SetMutation(string)
  | ClearMutation;

let initialState = {query: "", mutation: ""};

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | SetQuery(query) => {...state, query}
        | ClearQuery => {...state, query: ""}
        | SetMutation(mutation) => {...state, mutation}
        | ClearMutation => {...state, mutation: ""}
        },
      initialState,
    );

  let executeQuery = () =>
    Client.executeQuery(~client, ~query=queryRequest, ())
    |> Wonka.forEach((. response) =>
         dispatch(SetQuery(Js.Json.stringifyWithSpace(response##data, 2)))
       );

  let executeMutation = () =>
    Client.executeMutation(~client, ~mutation=mutationRequest, ())
    |> Wonka.forEach((. response) =>
         dispatch(
           SetMutation(Js.Json.stringifyWithSpace(response##data, 2)),
         )
       );
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
          }|}
          ->str
        </div>
        <button
          className={Styles.colors.query->Styles.button}
          onClick={_event => executeQuery()}>
          "Execute Query"->str
        </button>
      </section>
      {switch (String.length(state.query)) {
       | 0 => React.null
       | _ =>
         <section className=Styles.section>
           <span className=Styles.title> "Result"->str </span>
           <div className={Styles.colors.query->Styles.code}>
             state.query->str
           </div>
           <button
             className={Styles.colors.query->Styles.button}
             onClick={_event => dispatch(ClearQuery)}>
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
          onClick={_event => executeMutation()}>
          "Execute Mutation"->str
        </button>
      </section>
      {switch (String.length(state.mutation)) {
       | 0 => React.null
       | _ =>
         <section className=Styles.section>
           <span className=Styles.title> "Result"->str </span>
           <div className={"db4d3f"->Styles.code}> state.mutation->str </div>
           <button
             className={"db4d3f"->Styles.button}
             onClick={_event => dispatch(ClearMutation)}>
             "Clear Mutation"->str
           </button>
         </section>
       }}
    </div>
  </div>;
};