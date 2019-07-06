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
  <div className=PreviewerStyles.previewer>
    <div className=PreviewerStyles.side>
      <section className=PreviewerStyles.section>
        <span className=PreviewerStyles.title> "Query"->React.string </span>
        <div className={PreviewerStyles.colors.query->PreviewerStyles.code}>
          {|query dogs {
  dogs {
    name
    breed
    likes
  }
}|}
          ->React.string
        </div>
        <button
          className={PreviewerStyles.colors.query->PreviewerStyles.button}
          onClick={_event => executeQuery()}>
          "Execute Query"->React.string
        </button>
      </section>
      {switch (String.length(state.query)) {
       | 0 => React.null
       | _ =>
         <section className=PreviewerStyles.section>
           <span className=PreviewerStyles.title>
             "Result"->React.string
           </span>
           <div className={PreviewerStyles.colors.query->PreviewerStyles.code}>
             state.query->React.string
           </div>
           <button
             className={PreviewerStyles.colors.query->PreviewerStyles.button}
             onClick={_event => dispatch(ClearQuery)}>
             "Clear Query"->React.string
           </button>
         </section>
       }}
    </div>
    <div className=PreviewerStyles.side>
      <section className=PreviewerStyles.section>
        <span className=PreviewerStyles.title> "Mutation"->React.string </span>
        <div className={PreviewerStyles.colors.mutation->PreviewerStyles.code}>
          {|mutation likeDog($key: ID!) {
  likeDog(key: $key) {
    name
    breed
    likes
  }
}|}
          ->React.string
        </div>
        <button
          className={PreviewerStyles.colors.mutation->PreviewerStyles.button}
          onClick={_event => executeMutation()}>
          "Execute Mutation"->React.string
        </button>
      </section>
      {switch (String.length(state.mutation)) {
       | 0 => React.null
       | _ =>
         <section className=PreviewerStyles.section>
           <span className=PreviewerStyles.title>
             "Result"->React.string
           </span>
           <div
             className={PreviewerStyles.colors.mutation->PreviewerStyles.code}>
             state.mutation->React.string
           </div>
           <button
             className={
               PreviewerStyles.colors.mutation->PreviewerStyles.button
             }
             onClick={_event => dispatch(ClearMutation)}>
             "Clear Mutation"->React.string
           </button>
         </section>
       }}
    </div>
  </div>;
};
