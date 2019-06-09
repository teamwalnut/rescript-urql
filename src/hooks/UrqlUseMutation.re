[@bs.deriving abstract]
type useMutationResponseJs('a) = {
  fetching: bool,
  [@bs.optional]
  data: 'a,
  [@bs.optional]
  error: UrqlCombinedError.t,
};

type useMutationResponse('a) = {
  fetching: bool,
  data: option('a),
  error: option(UrqlCombinedError.t),
  response: UrqlTypes.response('a),
};

type executeMutation =
  option(Js.Json.t) => Js.Promise.t(UrqlTypes.operationResult);

[@bs.module "urql"]
external useMutationJs:
  (~query: string) => (useMutationResponseJs('a), executeMutation) =
  "useMutation";

let useMutationResponseToRecord = (result: useMutationResponseJs('a)) => {
  let data = result->dataGet;
  let error = result->errorGet;
  let fetching = result->fetchingGet;

  let response: UrqlTypes.response('a) =
    switch (fetching, data, error) {
    | (true, _, _) => Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

let useMutation = (~query: string) => {
  let (useMutationResponseJs, executeMutation) = useMutationJs(~query);
  let useMutationResponse =
    useMutationResponseJs |> useMutationResponseToRecord;
  (useMutationResponse, executeMutation);
};