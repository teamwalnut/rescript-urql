type executeMutationJs =
  (option(Js.Json.t), option(UrqlClientTypes.partialOperationContextJs)) =>
  Js.Promise.t(UrqlClientTypes.operationResult);

[@bs.module "urql"]
external useMutationJs:
  string =>
  (UrqlTypes.jsHookResponse(Js.Json.t, 'extensions), executeMutationJs) =
  "useMutation";

let useMutation = (~request) => {
  let (responseJs, executeMutationJs) = useMutationJs(request##query);

  let response =
    UrqlResponse.urqlResponseToReason(
      ~response=responseJs,
      ~parse=request##parse,
    );

  let executeMutation = (~context=?, ()) => {
    let ctx = UrqlClientTypes.decodeOperationRequestPolicy(context);
    executeMutationJs(Some(request##variables), ctx);
  };

  (response, executeMutation);
};

let useDynamicMutation = definition => {
  let (parse, query, composeVariables) = definition;
  let (responseJs, executeMutationJs) = useMutationJs(query);

  let response =
    UrqlResponse.urqlResponseToReason(~response=responseJs, ~parse);

  let executeMutation = (~context=?, ()) => {
    let ctx = UrqlClientTypes.decodeOperationRequestPolicy(context);
    composeVariables(request => executeMutationJs(Some(request), ctx));
  };

  (response, executeMutation);
};
