type useMutationResponse('response) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('response),
};

let useMutation:
  (~request: UrqlTypes.request('response)) =>
  (
    useMutationResponse('response),
    unit => Js.Promise.t(UrqlTypes.operationResult),
  );