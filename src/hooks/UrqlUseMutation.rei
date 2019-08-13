open UrqlTypes;

let useMutation:
  (~request: request('response)) =>
  (
    hookResponse('response),
    unit => Js.Promise.t(UrqlClient.Types.operationResult),
  );
