open UrqlTypes;

let urqlResponseToReason = (parse: Js.Json.t => 'response, result: jsResponse) => {
  let data = result->dataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->errorGet
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->fetchingGet;

  let response =
    switch (fetching, data, error) {
    | (true, _, _) => Fetching
    | (false, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};
