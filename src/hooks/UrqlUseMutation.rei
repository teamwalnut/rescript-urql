let useMutation:
  (~request: UrqlTypes.request('response)) =>
  (
    UrqlTypes.hookResponse('response),
    unit => Js.Promise.t(UrqlClient.ClientTypes.operationResult),
  );

let useDynamicMutation:
  (
    (
      // `parse`
      Js.Json.t => 'a,
      // `query`
      string,
      // `composeVariables`
      (Js.Json.t => Js.Promise.t(UrqlClient.ClientTypes.operationResult)) =>
      'b,
    )
  ) =>
  (UrqlTypes.hookResponse('a), 'b);
