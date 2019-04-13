[@bs.deriving jsConverter]
type requestPolicy = [
  | [@bs.as "cache-first"] `CacheFirst
  | [@bs.as "cache-only"] `CacheOnly
  | [@bs.as "network-only"] `NetworkOnly
  | [@bs.as "cache-and-network"] `CacheAndNetwork
];

[@bs.deriving abstract]
type graphqlRequest = {
  key: int,
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
};

[@bs.deriving abstract]
type executionResult('a) = {
  [@bs.optional]
  errors: array(UrqlCombinedError.graphqlError),
  [@bs.optional]
  data: 'a,
};

type response('a) =
  | Fetching
  | Data('a)
  | Error(UrqlCombinedError.t)
  | NotFound;