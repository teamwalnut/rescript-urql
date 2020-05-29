type executeQueryJs =
  option(UrqlClientTypes.PartialOperationContextJs.t) => unit;

type useQueryResponseJs('extensions) = (
  UrqlTypes.jsHookResponse(Js.Json.t, 'extensions),
  executeQueryJs,
);

type executeQuery =
  (~context: UrqlClientTypes.partialOperationContext=?, unit) => unit;

type useQueryResponse('response, 'extensions) = (
  UrqlTypes.hookResponse('response, 'extensions),
  executeQuery,
);

type useQueryArgs = {
  query: string,
  variables: Js.Json.t,
  requestPolicy: option(string),
  pause: option(bool),
  context: option(UrqlClientTypes.PartialOperationContextJs.t),
  pollInterval: option(int),
};

[@bs.module "urql"]
external useQueryJs: useQueryArgs => useQueryResponseJs('extensions) =
  "useQuery";

/**
 * The useQuery hook.
 *
 * Accepts the following arguments:
 *
 * request – a Js.t containing the query and variables corresponding
 * to the GraphQL query, and a parse function for decoding the JSON response.
 *
 * requestPolicy - the request policy used to execute the query.
 * Can be one of `CacheFirst, `CacheOnly, `NetworkOnly, and `CacheAndNetwork.
 *
 * pause - prevent eager execution of the query.
 * The query will only execute when pause becomes false.
 *
 * pollInterval – execute this query on a regular interval. Provided in unit ms.
 *
 * context – a partial operation context to alter the execution conditions of
 * the query.
 */;
let useQuery =
    (~request, ~requestPolicy=?, ~pause=?, ~pollInterval=?, ~context=?, ()) => {
  let query = request##query;
  let variables = request##variables;
  let parse = request##parse;
  let rp =
    UrqlUseSemanticGuarantee.useSemanticGuarantee(
      () => requestPolicy->Belt.Option.map(UrqlTypes.requestPolicyToJs),
      requestPolicy,
    );
  let ctx =
    UrqlUseSemanticGuarantee.useSemanticGuarantee(
      () => context->UrqlClientTypes.decodePartialOperationContext,
      context,
    );

  let args = {
    query,
    variables,
    requestPolicy: rp,
    pause,
    pollInterval,
    context: ctx,
  };

  let (stateJs, executeQueryJs) = useQueryJs(args);
  let hasExecuted = React.useRef(false);

  let state =
    React.useMemo3(
      () =>
        UrqlResponse.urqlResponseToReason(
          ~response=stateJs,
          ~parse,
          ~hasExecuted=
            hasExecuted->React.Ref.current
            || !pause->Belt.Option.getWithDefault(false),
        ),
      (stateJs, parse, pause),
    );

  let executeQuery =
    React.useMemo1(
      ((), ~context=?, ()) => {
        if (!hasExecuted->React.Ref.current) {
          hasExecuted->React.Ref.setCurrent(true);
        };

        let ctx = UrqlClientTypes.decodePartialOperationContext(context);
        executeQueryJs(ctx);
      },
      [|executeQueryJs|],
    );

  (state, executeQuery);
};
