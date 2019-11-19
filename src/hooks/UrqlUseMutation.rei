let useMutation:
  (~request: UrqlTypes.request('response)) =>
  (
    UrqlTypes.hookResponse('response),
    unit => Js.Promise.t(UrqlClient.ClientTypes.operationResult),
  );

module type MutationConfig = {
  type t;
  let query: string;
  let parse: Js.Json.t => t;
};

module type MakeMutationType =
  (Mutation: MutationConfig) =>
   {
    let useMutation:
      unit =>
      (
        UrqlTypes.hookResponse(Mutation.t),
        option(Js.Json.t) =>
        Js.Promise.t(UrqlClient.ClientTypes.operationResult),
      );
  };

module MakeMutation: MakeMutationType;
