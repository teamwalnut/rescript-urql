let useMutation:
  (~request: UrqlTypes.request('response)) =>
  (
    UrqlTypes.hookResponse('response),
    unit => Js.Promise.t(UrqlClient.ClientTypes.operationResult),
  );

let useDynamicMutation:
  (~query: string, ~parse: Js.Json.t => 'response) =>
  (
    UrqlTypes.hookResponse('response),
    {.. "variables": Js.Json.t} =>
    Js.Promise.t(UrqlClient.ClientTypes.operationResult),
  );
