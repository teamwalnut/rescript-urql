type executeMutationJs =
  (Js.Json.t, Types.partialOperationContext) =>
  Js.Promise.t(Types.operationResult);

type executeMutation =
  (
    ~fetchOptions: Fetch.requestInit=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~url: string=?,
    ~meta: Types.operationDebugMeta=?,
    ~pollInterval: int=?,
    unit
  ) =>
  Js.Promise.t(Types.operationResult);

type useMutationResponseJs('extensions) = (
  Types.jsHookResponse(Js.Json.t, 'extensions),
  executeMutationJs,
);

type useMutationResponse('response, 'extensions) = (
  Types.hookResponse('response, 'extensions),
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
      () => Types.urqlResponseToReason(~response=stateJs, ~parse),
      (stateJs, parse),
    );

  let executeMutation =
    React.useMemo2(
      (
        (),
        ~fetchOptions=?,
        ~requestPolicy=?,
        ~url=?,
        ~meta=?,
        ~pollInterval=?,
        (),
      ) => {
        let ctx =
          Types.partialOperationContext(
            ~fetchOptions?,
            ~requestPolicy=?
              Belt.Option.map(requestPolicy, Types.requestPolicyToJs),
            ~url?,
            ~meta?,
            ~pollInterval?,
            (),
          );
        executeMutationJs(variables, ctx);
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
      () => Types.urqlResponseToReason(~response=stateJs, ~parse),
      (stateJs, parse),
    );

  let executeMutation =
      (~fetchOptions=?, ~requestPolicy=?, ~url=?, ~meta=?, ~pollInterval=?) => {
    let ctx =
      Types.partialOperationContext(
        ~fetchOptions?,
        ~requestPolicy=?
          Belt.Option.map(requestPolicy, Types.requestPolicyToJs),
        ~url?,
        ~meta?,
        ~pollInterval?,
        (),
      );
    composeVariables(variables => executeMutationJs(variables, ctx));
  };

  (state, executeMutation);
};
