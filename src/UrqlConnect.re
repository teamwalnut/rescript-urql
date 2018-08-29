/* Urql's <Connect /> component, which takes a render prop argument. */
[@bs.module "urql"] external connect: ReasonReact.reactClass = "Connect";

/* The pattern below allows us to type the query prop as a polymorphic variant,
   which can be either a single Urql query or an array of Urql queries. */
type jsUnsafe;

external toJsUnsafe: 'a => jsUnsafe = "%identity";

let unwrapQuery =
    (
      ~q:
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
  | None => toJsUnsafe("")
  };

/* Mutation types */
type mutationMap = Js.Dict.t(UrqlMutation.urqlMutation);

let unwrapMutation = (~m: option(mutationMap)) =>
  switch (m) {
  | Some(o) => o
  | None => Js.Dict.empty()
  };

/* Render prop types */
/* [@bs.deriving abstract] */
type error = {. "message": string};

/* Helper function to convert Urql errors to option */
let convertJsErrorToReason = (err: Js.Nullable.t(error)) =>
  err |> Js.Nullable.toOption;

[@bs.deriving abstract]
type skipFetch = {
  [@bs.optional]
  skipFetch: bool,
};

type refetch = (~options: skipFetch, ~initial: bool=?) => unit;

type refreshAllFromCache = unit => unit;

type response('data) =
  | Loading
  | Data('data)
  | Error(error);

/* [@bs.deriving abstract] */
type renderArgs('data) = {
  .
  "response": response('data),
  "fetching": bool,
  "loaded": bool,
  "data": Js.Nullable.t('data),
  "error": Js.Nullable.t(error),
  "refetch": refetch,
  "refreshAllFromCache": refreshAllFromCache,
};

/* [@bs.deriving abstract] */
type renderArgsJs('data) = {
  .
  "fetching": bool,
  "loaded": bool,
  "data": Js.Nullable.t('data),
  "error": Js.Nullable.t(error),
  "refetch": refetch,
  "refreshAllFromCache": refreshAllFromCache,
};

let urqlDataToVariant = (urqlData: renderArgsJs('data)) => {
  Js.log2("error", urqlData##error);
  Js.log2("data", urqlData##data);
  let response =
    switch (
      urqlData##fetching,
      urqlData##data |> Js.Nullable.toOption,
      urqlData##error |> Js.Nullable.toOption,
    ) {
    | (true, _, _) => Loading
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => Error({"message": "No data"})
    };

  Js.Obj.assign(urqlData, {"response": response});
};

type siRes;

type siData;

type shouldInvalidate =
  option(
    (
      ~changedTypes: array(string),
      ~typenames: array(string),
      ~response: siRes,
      ~data: siData
    ) =>
    bool,
  );

let component = ReasonReact.statelessComponent("Connect");

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
      ~shouldInvalidate: shouldInvalidate=?,
      _children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=connect,
    ~props={
      "query": unwrapQuery(~q=query),
      "mutation": unwrapMutation(~m=mutation),
      "cache": cache,
      "typeInvalidation": typeInvalidation,
      "shouldInvalidate": shouldInvalidate,
      "children": result => result |> urqlDataToVariant |> render,
    },
    _children,
  );