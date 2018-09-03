/* Urql's <Connect /> component, which takes a render prop argument. */
[@bs.module "urql"] external connect: ReasonReact.reactClass = "Connect";
let component = ReasonReact.statelessComponent("Connect");

/* The pattern below allows us to type the query prop as a polymorphic variant,
   which can be either a single Urql query or an array of Urql queries. */
type t;
type jsUnsafe;

external toJsUnsafe: 'a => jsUnsafe = "%identity";

/* Query helpers */
let unwrapQuery =
    (
      q:
        option(
          [
            | `Query(UrqlQuery.urqlQuery)
            | `QueryArray(array(UrqlQuery.urqlQuery))
          ],
        ),
    ) =>
  switch (q) {
  | Some(`Query(q)) => toJsUnsafe(q)
  | Some(`QueryArray(qa)) => toJsUnsafe(qa)
  | None => toJsUnsafe(None)
  };

/* Mutation helpers */
type mutationMap = Js.Dict.t(UrqlMutation.urqlMutation);

let unwrapMutation = (m: option(mutationMap)) =>
  switch (m) {
  | Some(o) => o
  | None => Js.Dict.empty()
  };

/* Cache helpers on the render prop */
type error = {. "message": string};

[@bs.deriving abstract]
type refetchOptions = {
  [@bs.optional]
  skipFetch: bool,
};

type refetch = (~options: refetchOptions, ~initial: bool=?) => unit;

type refreshAllFromCache = unit => unit;

/* Response variant on the render prop */
type response('data) =
  | Loading
  | Data('data)
  | Error(error);

/* Render prop conversion types */
type renderArgsJs('data) = {
  .
  "fetching": bool,
  "loaded": bool,
  "data": Js.Nullable.t('data),
  "error": Js.Nullable.t(error),
  "refetch": refetch,
  "refreshAllFromCache": refreshAllFromCache,
};

type renderArgs('data) = {
  .
  "response": response('data),
  "fetching": bool,
  "loaded": bool,
  "data": option('data),
  "error": option(error),
  "refetch": refetch,
  "refreshAllFromCache": refreshAllFromCache,
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
    {"response": response, "data": data, "error": error},
  );
};

type siRes;

type shouldInvalidate('data) =
  option(
    (
      ~changedTypes: array(string),
      ~typenames: array(string),
      ~response: siRes,
      ~data: 'data
    ) =>
    bool,
  );

let make =
    (
      ~query:
         option(
           [
             | `Query(UrqlQuery.urqlQuery)
             | `QueryArray(array(UrqlQuery.urqlQuery))
           ],
         )=?,
      ~mutation: option(mutationMap)=?,
      ~render: renderArgs('data) => ReasonReact.reactElement,
      ~cache: bool=true,
      ~typeInvalidation: bool=true,
      ~shouldInvalidate: shouldInvalidate('data)=?,
      _children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=connect,
    ~props={
      "query": unwrapQuery(query),
      "mutation": unwrapMutation(mutation),
      "cache": cache,
      "typeInvalidation": typeInvalidation,
      "shouldInvalidate": shouldInvalidate,
      "children": result => result |> urqlDataToVariant |> render,
    },
    _children,
  );