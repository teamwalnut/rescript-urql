type partialOperationContextFn = option(UrqlTypes.partialOperationContext) => unit;
type useQueryState('a) = {
  fetching: bool,
  data: option('a),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('a)
}
type useQueryResponse('a) = (useQueryState('a), partialOperationContextFn);

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
