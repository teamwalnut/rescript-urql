[@bs.module "urql"] external connect: ReasonReact.reactClass = "Connect";
let component = ReasonReact.statelessComponent("Connect");

/* Mutation map. */
type mutationMap = Js.Dict.t(UrqlMutation.urqlMutation);

/* Error type and No Data error. */
[@bs.deriving abstract]
type error = {message: string};
let noData = error(~message="No Data");

/* Refetch types. */
[@bs.deriving abstract]
type refetchOptions = {
  [@bs.optional]
  skipCache: bool,
};

type refetch = (~options: refetchOptions, ~initial: bool=?) => unit;
type refreshAllFromCache = unit => unit;

/* Response variant on the render prop. */
type response('data) =
  | Loading
  | Data('data)
  | Error(error);

/* Render prop conversion types. */
[@bs.deriving jsConverter]
type cache('store, 'value) = {
  invalidate: (~query: UrqlQuery.urqlQuery=?) => Js.Promise.t(unit),
  invalidateAll: unit => Js.Promise.t(unit),
  read: (~query: UrqlQuery.urqlQuery) => Js.Promise.t('value),
  update:
    (~callback: (~store: 'store, ~key: string, ~value: 'value) => unit) =>
    Js.Promise.t(unit),
};

/* Arguments passed to the render prop. */
type renderArgs('data, 'store, 'value) = {
  .
  "response": response('data),
  "fetching": bool,
  "loaded": bool,
  "data": option('data),
  "error": option(error),
  "refetch": refetch,
  "refreshAllFromCache": refreshAllFromCache,
  "cache": cache('store, 'value),
};

let urqlDataToVariant = urqlData => {
  let data = urqlData##data |> Js.Nullable.toOption;
  let error = urqlData##error |> Js.Nullable.toOption;

  let response =
    switch (urqlData##loaded, data, error) {
    | (false, _, None) => Loading
    | (true, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (true, _, Some(error)) => Error(error)
    | (true, None, None) => Error(noData)
    };
  Js.Obj.assign(
    urqlData,
    {
      "response": response,
      "data": data,
      "error": error,
      "cache": urqlData##cache |> cacheFromJs,
    },
  );
};

type shouldInvalidate('mutation, 'data) =
  option(
    (
      ~changedTypes: array(string),
      ~typenames: array(string),
      ~response: 'mutation,
      ~data: 'data
    ) =>
    bool,
  );

let make =
    (
      ~query: option(UrqlQuery.urqlQuery)=?,
      ~mutation: mutationMap=Js.Dict.empty(),
      ~render: renderArgs('data, 'store, 'value) => ReasonReact.reactElement,
      ~cache: bool=true,
      ~typeInvalidation: bool=true,
      ~shouldInvalidate: shouldInvalidate('mutation, 'data)=?,
      _children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=connect,
    ~props={
      "query": query,
      "mutation": mutation,
      "cache": cache,
      "typeInvalidation": typeInvalidation,
      "shouldInvalidate": shouldInvalidate,
      "children": result => result |> urqlDataToVariant |> render,
    },
    _children,
  );