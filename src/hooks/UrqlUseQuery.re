type useQueryArgs = {
  query: string,
  variables: Js.Json.t,
  requestPolicy: option(string),
  pause: option(bool),
  context: option(UrqlClientTypes.partialOperationContextJs),
  pollInterval: option(int),
};

type executeQueryJs =
  option(UrqlClientTypes.partialOperationContextJs) => unit;

type executeQuery =
  (~context: UrqlClientTypes.partialOperationContext=?, unit) => unit;

/* The response to useQuery on the JavaScript side. */
type useQueryResponseJs('extensions) = (
  UrqlTypes.jsHookResponse(Js.Json.t, 'extensions),
  executeQueryJs,
);

/**
 * The response to useQuery – a two dimensional tuple containing
 * the result of executing the query and a function for re-executing
 * the query imperatively.
 */
type useQueryResponse('response, 'extensions) = (
  UrqlTypes.hookResponse('response, 'extensions),
  executeQuery,
);

[@bs.module "urql"]
external useQueryJs: useQueryArgs => useQueryResponseJs('extensions) =
  "useQuery";

let useQuery =
    (~request, ~requestPolicy=?, ~pause=?, ~pollInterval=?, ~context=?, ()) => {
  let args = {
    query: request##query,
    variables: request##variables,
    requestPolicy:
      requestPolicy->Belt.Option.map(UrqlTypes.requestPolicyToJs),
    pause,
    pollInterval,
    context: UrqlClientTypes.decodeOperationRequestPolicy(context),
  };

  let (responseJs, executeQueryJs) = useQueryJs(args);

  let response =
    UrqlResponse.urqlResponseToReason(
      ~response=responseJs,
      ~parse=request##parse,
    );
  let executeQuery = (~context=?, ()) => {
    let ctx = UrqlClientTypes.decodeOperationRequestPolicy(context);
    executeQueryJs(ctx);
  };

  (response, executeQuery);
};
