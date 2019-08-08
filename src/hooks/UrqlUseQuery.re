open UrqlTypes;
open UrqlConverters;

[@bs.deriving abstract]
type useQueryArgs = {
  query: string,
  variables: Js.Json.t,
  [@bs.optional]
  requestPolicy,
  [@bs.optional]
  pause: bool,
};

type partialOperationContextFn = option(partialOperationContext) => unit;
type useQueryResponseJs = (jsResponse, partialOperationContextFn);

type useQueryResponse('response) = (
  hookResponse('response),
  partialOperationContextFn,
);

[@bs.module "urql"]
external useQueryJs: useQueryArgs => useQueryResponseJs = "useQuery";

let useQuery = (~request, ~requestPolicy=?, ~pause=?, ()) => {
  let args =
    useQueryArgs(
      ~query=request##query,
      ~variables=request##variables,
      ~requestPolicy?,
      ~pause?,
      (),
    );
  let (state, executeQuery) = useQueryJs(args);
  let state_record =
    React.useMemo2(
      () => state |> urqlResponseToReason(request##parse),
      (state, request##parse),
    );

  (state_record, executeQuery);
};
