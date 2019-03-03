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