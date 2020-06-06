/**
 * A function for converting the response to an urql hook from its
 * JavaScript representation to a typed Reason record.
 */
let urqlResponseToReason = (~response, ~parse) => {
  let data = UrqlTypes.(response.data)->Belt.Option.map(parse);
  let error =
    response.error->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = response.fetching;
  let extensions = response.extensions;

  let response =
    switch (fetching, data, error) {
    | (true, None, _) => UrqlTypes.Fetching
    | (_, Some(data), _) => Data(data)
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) => NotFound
    };

  UrqlTypes.{fetching, data, error, response, extensions};
};
