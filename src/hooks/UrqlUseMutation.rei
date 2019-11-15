let useMutation:
  (~request: UrqlTypes.request('response)) =>
  (
    UrqlTypes.hookResponse('response),
    unit => Js.Promise.t(UrqlClient.ClientTypes.operationResult),
  );

module type MutationConfig = {
  type t;
  let parse: Js.Json.t => t;
  let query: string;
};

module type MakeType =
  (Mutation: MutationConfig) =>
   {
    let useMutation:
      unit =>
      (
        UrqlTypes.hookResponse(Mutation.t),
        Js.Json.t => Js.Promise.t(UrqlClient.ClientTypes.operationResult),
      );
  };

module Make: MakeType;
