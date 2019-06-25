type partialOperationContextFn =
  option(UrqlTypes.partialOperationContext) => unit;

type useQueryState('response) = {
  fetching: bool,
  data: option('response),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('response),
};

type useQueryResponse('response) = (
  useQueryState('response),
  partialOperationContextFn,
);

let useQuery:
  (
    ~request: {
                .
                "parse": Js.Json.t => 'response,
                "query": string,
                "variables": Js.Json.t,
              },
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~pause: bool=?,
    unit
  ) =>
  useQueryResponse('response);
