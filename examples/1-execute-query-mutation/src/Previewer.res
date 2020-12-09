open ReasonUrql

let client = Client.make(~url="https://formidadog-ql.netlify.app/graphql", ())

module GetAllDogs = %graphql(
  `
  query dogs {
    dogs {
      name
      breed
      likes
    }
  }
`
)

module LikeDog = %graphql(
  `
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      name
      breed
      likes
    }
  }
`
)

type state = {
  query: option<Js.Json.t>,
  mutation: option<Js.Json.t>,
  fetchingQuery: bool,
  fetchingMutation: bool,
}

type action =
  | SetQuery(Js.Json.t)
  | ClearQuery
  | SetMutation(Js.Json.t)
  | ClearMutation
  | SetQueryFetching(bool)
  | SetMutationFetching(bool)

let initialState = {
  query: None,
  mutation: None,
  fetchingQuery: false,
  fetchingMutation: false,
}

@react.component
let make = () => {
  let (state, dispatch) = React.useReducer((state, action) =>
    switch action {
    | SetQuery(query) => {...state, query: Some(query)}
    | ClearQuery => {...state, query: None}
    | SetMutation(mutation) => {...state, mutation: Some(mutation)}
    | ClearMutation => {...state, mutation: None}
    | SetQueryFetching(fetching) => {...state, fetchingQuery: fetching}
    | SetMutationFetching(fetching) => {
        ...state,
        fetchingMutation: fetching,
      }
    }
  , initialState)

  let executeQuery = () => {
    dispatch(SetQueryFetching(true))

    Client.executeQuery(
      ~client,
      ~query=module(GetAllDogs),
      ~requestPolicy=#CacheAndNetwork,
      (),
    ) |> Wonka.subscribe((. data) => {
      dispatch(SetQueryFetching(false))

      switch {
        open Types
        data.response
      } {
      | Data(d) => dispatch(SetQuery(d->GetAllDogs.serialize->GetAllDogs.toJson))
      | Error(_e) => dispatch(SetQuery(Js.Json.string("An error occured")))
      | _ => ()
      }
    })
  }

  let executeMutation = () => {
    dispatch(SetMutationFetching(true))

    Client.executeMutation(
      ~client,
      ~mutation=module(LikeDog),
      {
        open LikeDog
        {key: "VmeRTX7j-"}
      },
    ) |> Wonka.subscribe((. data) => {
      dispatch(SetMutationFetching(false))

      switch {
        open Types
        data.response
      } {
      | Data(d) => dispatch(SetMutation(d->LikeDog.serialize->LikeDog.toJson))
      | Error(_e) => dispatch(SetMutation(Js.Json.string("An error occurred")))
      | _ => ()
      }
    })
  }

  <div className="previewer">
    <div className="quadrant">
      <span className="quadrant__title"> {"Query"->React.string} </span>
      <div className="quadrant__code quadrant__code--teal">
        {`query dogs {
  dogs {
    name
    breed
    likes
  }
}`->React.string}
      </div>
      <button
        className="quadrant__button quadrant__button--teal"
        onClick={_event => executeQuery() |> ignore}>
        {"Execute Query"->React.string}
      </button>
    </div>
    <div className="quadrant">
      {switch (state.query, state.fetchingQuery) {
      | (None, false) => React.null
      | (_, true) => <span className="quadrant__title"> {"Loading"->React.string} </span>
      | _ => <>
          <span className="quadrant__title"> {"Result"->React.string} </span>
          <div className="quadrant__code quadrant__code--teal">
            {state.query
            ->Belt.Option.getWithDefault(Js.Json.string(""))
            ->Js.Json.stringifyWithSpace(2)
            ->React.string}
          </div>
          <button
            className="quadrant__button quadrant__button--teal"
            onClick={_event => dispatch(ClearQuery)}>
            {"Clear Query"->React.string}
          </button>
        </>
      }}
    </div>
    <div className="quadrant">
      <span className="quadrant__title"> {"Mutation"->React.string} </span>
      <div className="quadrant__code quadrant__code--orange">
        {`mutation likeDog($key: ID!) {
  likeDog(key: $key) {
    name
    breed
    likes
  }
}`->React.string}
      </div>
      <button
        className="quadrant__button quadrant__button--orange"
        onClick={_event => executeMutation() |> ignore}>
        {"Execute Mutation"->React.string}
      </button>
    </div>
    <div className="quadrant">
      {switch (state.mutation, state.fetchingMutation) {
      | (None, false) => React.null
      | (_, true) => <span className="quadrant__title"> {"Loading"->React.string} </span>

      | _ => <>
          <span className="quadrant__title"> {"Result"->React.string} </span>
          <div className="quadrant__code quadrant__code--orange">
            {state.mutation
            ->Belt.Option.getWithDefault(Js.Json.string(""))
            ->Js.Json.stringifyWithSpace(2)
            ->React.string}
          </div>
          <button
            className="quadrant__button quadrant__button--orange"
            onClick={_event => dispatch(ClearMutation)}>
            {"Clear Mutation"->React.string}
          </button>
        </>
      }}
    </div>
  </div>
}
