open ReasonUrql;

let client =
  Client.make(~url="https://formidadog-ql.netlify.app/graphql", ());

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

let queryRequest = GetAllDogs.make();

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

let mutationRequest = LikeDog.make(~key="VmeRTX7j-", ());

type state = {
  query: string,
  mutation: string,
  fetchingQuery: bool,
  fetchingMutation: bool,
};

type action =
  | SetQuery(string)
  | ClearQuery
  | SetMutation(string)
  | ClearMutation
  | SetQueryFetching(bool)
  | SetMutationFetching(bool);

let initialState = {
  query: "",
  mutation: "",
  fetchingQuery: false,
  fetchingMutation: false,
};

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
        | SetQueryFetching(fetching) => {...state, fetchingQuery: fetching}
        | SetMutationFetching(fetching) => {
            ...state,
            fetchingMutation: fetching,
          }
        },
      initialState,
    );

  let executeQuery = () => {
    dispatch(SetQueryFetching(true));

    Client.executeQuery(
      ~client,
      ~request=queryRequest,
      ~requestPolicy=`CacheAndNetwork,
      (),
    )
    |> Wonka.subscribe((. data) => {
         dispatch(SetQueryFetching(false));

         switch (Client.(data.response)) {
         | Data(d) =>
           switch (Js.Json.stringifyAny(d)) {
           | Some(s) => dispatch(SetQuery(s))
           | None => ()
           }
         | Error(e) =>
           switch (Js.Json.stringifyAny(e)) {
           | Some(s) => dispatch(SetQuery(s))
           | None => ()
           }
         | _ => ()
         };
       });
  };

  let executeMutation = () => {
    dispatch(SetMutationFetching(true));

    Client.executeMutation(~client, ~request=mutationRequest, ())
    |> Wonka.subscribe((. data) => {
         dispatch(SetMutationFetching(false));

         switch (Client.(data.response)) {
         | Data(d) =>
           switch (Js.Json.stringifyAny(d)) {
           | Some(s) => dispatch(SetMutation(s))
           | None => ()
           }
         | Error(e) =>
           switch (Js.Json.stringifyAny(e)) {
           | Some(s) => dispatch(SetMutation(s))
           | None => ()
           }
         | _ => ()
         };
       });
  };

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
          onClick={_event => executeQuery() |> ignore}>
          "Execute Query"->React.string
        </button>
      </section>
      {switch (String.length(state.query), state.fetchingQuery) {
       | (0, false) => React.null
       | (_, true) =>
         <section className=PreviewerStyles.section>
           <span className=PreviewerStyles.title>
             "Loading"->React.string
           </span>
         </section>
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
          onClick={_event => executeMutation() |> ignore}>
          "Execute Mutation"->React.string
        </button>
      </section>
      {switch (String.length(state.mutation), state.fetchingMutation) {
       | (0, false) => React.null
       | (_, true) =>
         <section className=PreviewerStyles.section>
           <span className=PreviewerStyles.title>
             "Loading"->React.string
           </span>
         </section>
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
