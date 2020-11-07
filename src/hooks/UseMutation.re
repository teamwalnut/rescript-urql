type executeMutationJs('dataJs) =
  (Js.Json.t, Types.partialOperationContext) =>
  Js.Promise.t(Types.operationResultJs('dataJs));

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
  Js.Promise.t(Types.operationResult('data));

type useMutationResponseJs('dataJs) = (
  Types.Hooks.hookResponseJs('dataJs),
  executeMutationJs('dataJs),
);

type useMutationResponse('variables, 'data) = (
  Types.Hooks.hookResponse('data),
  executeMutation('variables, 'data),
);

[@bs.module "urql"]
external useMutationJs: string => useMutationResponseJs('dataJs) =
  "useMutation";

/**
 * The useMutation hook.
 */
let useMutation:
  type data variables.
    (
      ~mutation: (module Types.Operation with
                    type t = data and type t_variables = variables)
    ) =>
    useMutationResponse(variables, data) =
  (~mutation as (module Mutation)) => {
    let query = Mutation.query;
    let parse = Mutation.parse;

    let (stateJs, executeMutationJs) = useMutationJs(query);

    let state =
      React.useMemo2(
        () => Types.Hooks.hookResponseToReason(~response=stateJs, ~parse),
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
            Mutation.(variables->serializeVariables->variablesToJson),
            ctx,
          )
          ->Js.Promise.(
              then_(
                response =>
                  Types.operationResultToReason(~response, ~parse)->resolve,
                _,
              )
            );
        },
        [|executeMutationJs|],
      );

    (state, executeMutation);
  };
