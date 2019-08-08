open UrqlTypes;
open UrqlConverters;

type executeMutation = option(Js.Json.t) => Js.Promise.t(operationResult);

[@bs.module "urql"]
external useMutationJs: string => (jsResponse, executeMutation) =
  "useMutation";

let useMutation = (~request: request('response)) => {
  let (useMutationResponseJs, executeMutationJs) =
    useMutationJs(request##query);
  let useMutationResponse =
    React.useMemo2(
      () => useMutationResponseJs |> urqlResponseToReason(request##parse),
      (request##parse, useMutationResponseJs),
    );
  let executeMutation =
    React.useCallback1(
      () => executeMutationJs(Some(request##variables)),
      [|request##variables|],
    );
  (useMutationResponse, executeMutation);
};
