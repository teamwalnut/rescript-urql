/**
 * A function for converting the response to an urql hook from its
 * JavaScript representation to a typed Reason record.
 */
let urqlResponseToReason = (~response, ~parse, ~hasExecuted) => {
  let data = UrqlTypes.(response.data)->Belt.Option.map(parse);
  let error =
    response.error->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = response.fetching;
  let extensions = response.extensions;

  let response =
    switch (fetching, data, error, hasExecuted) {
    | (false, None, None, false) => UrqlTypes.Init
    | (true, None, _, _) => Fetching
    | (_, Some(data), _, _) => Data(data)
    | (false, _, Some(error), _) => Error(error)
    | (false, None, None, true) => Empty
    };

  UrqlTypes.{fetching, data, error, response, extensions};
};

/**
 * A function to convert the JS response from a client.execute*
 * methods to typed a Reason record.
 */
let urqlClientResponseToReason =
    (~response: UrqlClientTypes.operationResult, ~parse) => {
  let data = UrqlClientTypes.(response.data)->Belt.Option.map(parse);
  let error =
    response.error->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);

  let response =
    switch (data, error) {
    | (Some(data), _) => UrqlClientTypes.Data(data)
    | (None, Some(error)) => Error(error)
    | (None, None) => Empty
    };

  UrqlClientTypes.{data, error, response};
};
