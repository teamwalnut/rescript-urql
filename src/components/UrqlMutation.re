[@bs.module "urql"] external mutation: ReasonReact.reactClass = "Mutation";
let component = ReasonReact.statelessComponent("Mutation");

type mutationRenderPropsJs('a) = {
  .
  "fetching": bool,
  "data": Js.Nullable.t('a),
  "error": Js.Nullable.t(UrqlError.t),
  "executeMutation": Js.Json.t => Js.Promise.t('a),
};

type response('a) =
  | Fetching
  | Data('a)
  | Error(UrqlError.t)
  | NotFound;

type mutationRenderProps('a) = {
  fetching: bool,
  data: option('a),
  error: option(UrqlError.t),
  executeMutation: Js.Json.t => Js.Promise.t('a),
  response: response('a),
};

let urqlDataToRecord = (result: mutationRenderPropsJs('a)) => {
  let data = result##data |> Js.Nullable.toOption;
  let error = result##error |> Js.Nullable.toOption;

  let response: response('a) =
    switch (result##fetching, data, error) {
    | (true, _, _) => Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {
    fetching: result##fetching,
    data,
    error,
    executeMutation: result##executeMutation,
    response,
  };
};

let make =
    (
      ~query: string,
      children: mutationRenderProps('a) => ReasonReact.reactElement,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=mutation, ~props={"query": query}, result =>
    children(result->urqlDataToRecord)
  );