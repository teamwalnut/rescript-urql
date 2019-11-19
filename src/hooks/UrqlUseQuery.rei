type executeQuery =
  option(UrqlClient.ClientTypes.partialOperationContext) => unit;

type useQueryResponse('response) = (
  UrqlTypes.hookResponse('response),
  executeQuery,
);

let useQuery:
  (
    ~request: UrqlTypes.request('response),
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~pause: bool=?,
    unit
  ) =>
  useQueryResponse('response);

module type QueryConfig = {
  type t;
  let query: string;
  let parse: Js.Json.t => t;
};

module type MakeQueryType =
  (Query: QueryConfig) =>
   {
    let useQuery:
      (
        ~variables: Js.Json.t=?,
        ~requestPolicy: UrqlTypes.requestPolicy=?,
        ~pause: bool=?,
        unit
      ) =>
      (
        UrqlTypes.hookResponse(Query.t),
        React.callback(
          option(UrqlClient.ClientTypes.partialOperationContext),
          unit,
        ),
      );
  };

module MakeQuery: MakeQueryType;
