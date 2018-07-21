/* Urql's <Connect /> component, which takes a render prop argument. */
[@bs.module "urql"] external connect : ReasonReact.reactClass = "Connect";

/* The pattern below allows us to type the query prop as a polymorphic variant,
   which can be either a single Urql query or an array of Urql queries. */
type jsUnsafe;

external toJsUnsafe : 'a => jsUnsafe = "%identity";

let unwrapQuery =
    (
      ~q: option([ | `Query(Query.urqlQuery) | `QueryArray(array(Query.urqlQuery))]),
    ) =>
  switch (q) {
  | Some(`Query(q)) => toJsUnsafe(q)
  | Some(`QueryArray(qa)) => toJsUnsafe(qa)
  | None => toJsUnsafe("")
  };

/* Mutation types */
type mutationMap = Js.Dict.t(Mutation.urqlMutation);

let unwrapMutation = (~m: option(mutationMap)) =>
  switch (m) {
  | Some(o) => o
  | None => Js.Dict.empty()
  };

/* Render prop types */
/* [@bs.deriving abstract]
type errorObject = {
  message: string
};

[@bs.deriving accessors]
type error =
  | ErrorStr(string)
  | ErrorObj(errorObject)
  | Empty; */
[@bs.deriving abstract]
type error = {
  message: string,
};

[@bs.deriving abstract]
type skipFetch = {
  [@bs.optional]
  skipFetch: bool,
};
type refetch = (~options: skipFetch, ~initial: bool=?) => unit;

type refreshAllFromCache = unit => unit;

[@bs.deriving abstract]
type renderArgs('data) = {
  fetching: bool,
  loaded: bool,
  data: 'data,
  error: Js.Nullable.t(error),
  refetch,
  refreshAllFromCache
};

type siRes;
type siData;

type shouldInvalidate = option((~changedTypes: array(string), ~typenames: array(string), ~response: siRes, ~data: siData) => bool);

let component = ReasonReact.statelessComponent("Connect");

let make =
    (
      ~query: option([
         | `Query(Query.urqlQuery)
         | `QueryArray(array(Query.urqlQuery))
       ])=?,
      ~mutation: option(mutationMap)=?,
      ~renderProp:
         (~result: renderArgs('data)) => ReasonReact.reactElement,
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
      "children": renderProp,
    },
    _children,
  );
