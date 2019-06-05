type partialOperationContextFn = option(UrqlTypes.partialOperationContext) => unit;
type useQueryState('a) = {
  fetching: bool,
  data: option('a),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('a)
}
type useQueryResponse('a) = (useQueryState('a), partialOperationContextFn);

let useQuery: (~query: string, ~variables: 'a=?, ~requestPolicy: UrqlTypes.requestPolicy=?, ~pause: bool=?, unit) => useQueryResponse('b)
