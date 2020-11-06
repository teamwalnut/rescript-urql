type executeMutationJs('jsData) =
  (Js.Json.t, Types.partialOperationContext) =>
  Js.Promise.t(Types.operationResult('jsData));

type executeMutation('variables, 'data) =
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
    'variables
  ) =>
  Js.Promise.t(Client.clientResponse('data));

type useMutationResponseJs('jsData) = (
  Types.hookResponseJs('jsData),
  executeMutationJs('jsData),
);

type useMutationResponse('variables, 'data) = (
  Types.hookResponse('data),
  executeMutation('variables, 'data),
);

[@bs.module "urql"]
external useMutationJs: string => useMutationResponseJs('jsData) =
  "useMutation";

/**
 * The useMutation hook.
 */
let useMutation:
  type data variables.
    (
      ~query: (module Types.Operation with
                 type t = data and type t_variables = variables)
    ) =>
    useMutationResponse(variables, data) =
  (~query as (module Query)) => {
    let query = Query.query;
    let parse = Query.parse;

    let (stateJs, executeMutationJs) = useMutationJs(query);

    let state =
      React.useMemo2(
        () => Types.urqlResponseToReason(~response=stateJs, ~parse),
        (stateJs, parse),
      );

    let executeMutation =
      React.useMemo1(
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
          variables,
        ) => {
          let ctx =
            Types.partialOperationContext(
              ~additionalTypenames?,
              ~fetchOptions?,
              ~fetch?,
              ~url?,
              ~requestPolicy=?
                requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
              ~pollInterval?,
              ~meta?,
              ~suspense?,
              ~preferGetMethod?,
              (),
            );

          executeMutationJs(
            Query.(variables->serializeVariables->variablesToJson),
            ctx,
          )
          ->Js.Promise.(
              then_(
                response =>
                  Client.clientResponseToReason(~response, ~parse)->resolve,
                _,
              )
            );
        },
        [|executeMutationJs|],
      );

    (state, executeMutation);
  };
