[@bs.module "urql"]
external subscriptionComponent: ReasonReact.reactClass = "Subscription";

type subscriptionRenderPropsJs('a) = {
  .
  "fetching": bool,
  "data": Js.Nullable.t('a),
  "error": Js.Nullable.t(UrqlCombinedError.t),
};

type subscriptionRenderProps('a) = {
  fetching: bool,
  data: option('a),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('a),
};

let urqlDataToRecord = (result: subscriptionRenderPropsJs('a)) => {
  let data = result##data |> Js.Nullable.toOption;
  let error = result##error |> Js.Nullable.toOption;

  let response: UrqlTypes.response('a) =
    switch (result##fetching, data, error) {
    | (true, _, _) => Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching: result##fetching, data, error, response};
};

type handler('a, 'b) = ('a, 'b) => 'a;

[@bs.deriving abstract]
type jsProps('a, 'b) = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  handler: handler('a, 'b),
};

let make =
    (
      ~query,
      ~variables=?,
      ~handler=?,
      children: subscriptionRenderProps('a) => ReasonReact.reactElement,
    ) => {
  let props =
    switch (variables, handler) {
    | (Some(v), Some(h)) => jsProps(~query, ~variables=v, ~handler=h, ())
    | (Some(v), None) => jsProps(~query, ~variables=v, ())
    | (None, Some(h)) => jsProps(~query, ~handler=h, ())
    | (None, None) => jsProps(~query, ())
    };

  ReasonReact.wrapJsForReason(
    ~reactClass=subscriptionComponent, ~props, result =>
    result |> urqlDataToRecord |> children
  );
};