type executeMutationJs =
  (Js.Json.t, Types.partialOperationContext) =>
  Js.Promise.t(Types.operationResult);

type executeMutation =
  (
    ~additionalTypenames: array(string)=?,
    ~fetchOptions: Fetch.requestInit=?,
    ~fetch: (string, Fetch.requestInit) => Js.Promise.t(Fetch.response)=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~url: string=?,
    ~pollInterval: int=?,
    ~meta: Types.operationDebugMeta=?,
    ~suspense: bool=?,
    ~preferGetMethod: bool=?,
    unit
  ) =>
  Js.Promise.t(Types.operationResult);

type useMutationResponseJs = (
  Types.hookResponseJs(Js.Json.t),
  executeMutationJs,
);

type useMutationResponse('response) = (
  Types.hookResponse('response),
  executeMutation,
);

[@bs.module "urql"]
external useMutationJs: string => useMutationResponseJs = "useMutation";

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

  let client = UseClient.useClient();

  let (stateJs, executeMutationJs) = useMutationJs(query);

  let state =
    React.useMemo2(
      () => Types.urqlResponseToReason(~response=stateJs, ~parse),
      (stateJs, parse),
    );

  let executeMutation =
    React.useMemo3(
      (
        (),
        ~additionalTypenames=?,
        ~fetchOptions=?,
        ~fetch=?,
        ~requestPolicy=?,
        ~url=?,
        ~pollInterval=?,
        ~meta=?,
        ~suspense=?,
        ~preferGetMethod=?,
        (),
      ) => {
        let ctx: Types.partialOperationContext = {
          additionalTypenames,
          fetchOptions,
          fetch,
          requestPolicy:
            requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
          url: Some(url->Belt.Option.getWithDefault(client.url)),
          pollInterval,
          meta,
          suspense,
          preferGetMethod,
        };

        executeMutationJs(variables, ctx);
      },
      (executeMutationJs, variables, client),
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

  let client = UseClient.useClient();

  let state =
    React.useMemo2(
      () => Types.urqlResponseToReason(~response=stateJs, ~parse),
      (stateJs, parse),
    );

  let executeMutation =
      (
        ~additionalTypenames=?,
        ~fetchOptions=?,
        ~fetch=?,
        ~requestPolicy=?,
        ~url=?,
        ~pollInterval=?,
        ~meta=?,
        ~suspense=?,
        ~preferGetMethod=?,
      ) => {
    let ctx: Types.partialOperationContext = {
      additionalTypenames,
      fetchOptions,
      fetch,
      requestPolicy: requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
      url: Some(url->Belt.Option.getWithDefault(client.url)),
      pollInterval,
      meta,
      suspense,
      preferGetMethod,
    };

    composeVariables(variables => executeMutationJs(variables, ctx));
  };

  (state, executeMutation);
};
