[@bs.deriving abstract]
type useQueryState('a) = {
  fetching: bool,
  [@bs.optional] data: 'a,
  [@bs.optional] error: UrqlCombinedError.t
};

[@bs.deriving abstract]
type useQueryArgs('a) = {
  query: string,
  [@bs.optional] variables: 'a,
  [@bs.optional] requestPolicy: UrqlTypes.requestPolicy,
  [@bs.optional] pause: bool
}

type partialOperationContextFunc = option(UrqlTypes.partialOperationContext) => unit;
type useQueryResponse('a) = (useQueryState('a), partialOperationContextFunc);

[@bs.module "urql"]
external useQuery: (useQueryArgs('a)) => useQueryResponse('b) = "useQuery";
