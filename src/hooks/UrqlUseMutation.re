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

let useMutationResponseToRecord =
    (parse: Js.Json.t => 'response, result: useMutationResponseJs('a)) => {
  let data =
    switch (result->dataGet) {
    | None => None
    | Some(d) => Some(parse(d))
    };
  let error = result->errorGet;
  let fetching = result->fetchingGet;

  let response: UrqlTypes.response('response) =
    switch (fetching, data, error) {
    | (true, _, _) => Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

let useMutation =
    (
      ~request: {
         .
         "query": string,
         "variables": Js.Json.t,
         "parse": Js.Json.t => 'response,
       },
    ) => {
  let (useMutationResponseJs, executeMutation) =
    useMutationJs(~query=request##query);
  let useMutationResponse =
    useMutationResponseJs |> useMutationResponseToRecord(request##parse);
  (useMutationResponse, () => executeMutation(Some(request##variables)));
};
