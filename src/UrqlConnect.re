[@bs.module "urql"] external connect: ReasonReact.reactClass = "Connect";
let component = ReasonReact.statelessComponent("Connect");

/* Mutation helpers. */
type mutationMap = Js.Dict.t(UrqlMutation.urqlMutation);

let unwrapMutation = (m: option(mutationMap)) =>
  switch (m) {
  | Some(o) => o
  | None => Js.Dict.empty()
  };

/* Render prop types and helpers. */
[@bs.deriving abstract]
type error = {. "message": string};

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
type cache('queryData, 'store) = {
  invalidate: UrqlQuery.urqlQuery => UrqlClient.invalidate,
  invalidateAll: unit => UrqlClient.invalidateAll,
  read: UrqlQuery.urqlQuery => UrqlClient.read('queryData),
  update:
    ('store, string, 'queryData) => UrqlClient.update('queryData, 'store),
};

type renderArgs('data, 'queryData, 'store) = {
  .
  "response": response('data),
  "fetching": bool,
  "loaded": bool,
  "data": option('data),
  "error": option(error),
  "refetch": refetch,
  "refreshAllFromCache": refreshAllFromCache,
  "cache": cache('queryData, 'store),
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
    | (true, None, None) => Error({"message": "No data"})
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

type shouldInvalidate('mutationResponse, 'data) =
  option(
    (
      ~changedTypes: array(string),
      ~typenames: array(string),
      ~response: 'mutationResponse,
      ~data: 'data
    ) =>
    bool,
  );

let make =
    (
      ~query: option(UrqlQuery.urqlQuery)=?,
      ~mutation: option(mutationMap)=?,
      ~render:
         renderArgs('data, 'queryData, 'store) => ReasonReact.reactElement,
      ~cache: bool=true,
      ~typeInvalidation: bool=true,
      ~shouldInvalidate: shouldInvalidate('mutationResponse, 'data)=?,
      _children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=connect,
    ~props={
      "query": query,
      "mutation": unwrapMutation(mutation),
      "cache": cache,
      "typeInvalidation": typeInvalidation,
      "shouldInvalidate": shouldInvalidate,
      "children": result => result |> urqlDataToVariant |> render,
    },
    _children,
  );