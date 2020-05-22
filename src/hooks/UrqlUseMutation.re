type executeMutationJs =
  (option(Js.Json.t), option(UrqlClientTypes.PartialOperationContextJs.t)) =>
  Js.Promise.t(UrqlClientTypes.operationResult);

type executeMutation =
  (~context: UrqlClientTypes.partialOperationContext=?, unit) =>
  Js.Promise.t(UrqlClientTypes.operationResult);

type useMutationResponseJs('extensions) = (
  UrqlTypes.jsHookResponse(Js.Json.t, 'extensions),
  executeMutationJs,
);

type useMutationResponse('response, 'extensions) = (
  UrqlTypes.hookResponse('response, 'extensions),
  executeMutation,
);

[@bs.module "urql"]
external useMutationJs: string => useMutationResponseJs('extensions) =
  "useMutation";

/**
 * The useMutation hook.
 *
 * Accepts the following arguments:
 *
 * request – a Js.t containing the query and variables corresponding
 * to the GraphQL mutation, and a parse function for decoding the JSON response.
 */
let useMutation = (~request) => {
  let query = request##query;
  let variables = request##variables;
  let parse = request##parse;

  let (stateJs, executeMutationJs) = useMutationJs(query);

  let state =
    React.useMemo2(
      () => UrqlResponse.urqlResponseToReason(~response=stateJs, ~parse),
      (stateJs, parse),
    );

  let executeMutation =
    React.useMemo2(
      ((), ~context=?, ()) => {
        let ctx = UrqlClientTypes.decodePartialOperationContext(context);
        executeMutationJs(Some(variables), ctx);
      },
      (executeMutationJs, variables),
    );

  (state, executeMutation);
};

/**
 * The useDynamicMutation hook.
 *
 * Accepts the following arguments:
 *
 * definition – a tuple on the graphql_ppx_re module representing your mutation.
 * Allows for proper type inference of results on mutations executed with variables
 * passed at runtime.
 */
let useDynamicMutation = definition => {
  let (parse, query, composeVariables) = definition;
  let (stateJs, executeMutationJs) = useMutationJs(query);

  let state =
    React.useMemo2(
      () => UrqlResponse.urqlResponseToReason(~response=stateJs, ~parse),
      (stateJs, parse),
    );

  let executeMutation =
    React.useMemo2(
      ((), ~context=None) => {
        let ctx = UrqlClientTypes.decodePartialOperationContext(context);
        composeVariables(variables =>
          executeMutationJs(Some(variables), ctx)
        );
      },
      (composeVariables, executeMutationJs),
    );

  (state, executeMutation);
};
