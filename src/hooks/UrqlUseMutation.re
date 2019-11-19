/**
 * The type of executeMutation – a function returned by
 * useMutation to imperatively execute the mutation.
 */
type executeMutationJs =
  option(Js.Json.t) => Js.Promise.t(UrqlClient.ClientTypes.operationResult);

[@bs.module "urql"]
external useMutationJs:
  string => (UrqlTypes.jsResponse(Js.Json.t), executeMutationJs) =
  "useMutation";

/**
 * A function for converting the response to useQuery from the JavaScript
 * representation to a typed Reason record.
 */
let urqlResponseToReason =
    (parse: Js.Json.t => 'response, result: UrqlTypes.jsResponse(Js.Json.t))
    : UrqlTypes.hookResponse('response) => {
  let data =
    result->UrqlTypes.jsDataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->UrqlTypes.jsErrorGet
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->UrqlTypes.fetchingGet;

  let response =
    switch (fetching, data, error) {
    | (true, _, _) => UrqlTypes.Fetching
    | (false, _, Some(error)) => Error(error)
    | (false, Some(data), _) => Data(data)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

/**
 * The useMutation hook.
 *
 * Accepts the following arguments:
 *
 * request – a Js.t containing the query and variables corresponding
 * to the GraphQL mutation, and a parse function for decoding the JSON response.
 */;
let useMutation = (~request) => {
  let (responseJs, executeMutationJs) = useMutationJs(request##query);

  let response =
    React.useMemo2(
      () => responseJs |> urqlResponseToReason(request##parse),
      (request##parse, responseJs),
    );

  let executeMutation =
    React.useCallback2(
      () => executeMutationJs(Some(request##variables)),
      (executeMutationJs, request##variables),
    );

  (response, executeMutation);
};

/**
 * The functor implementation of useMutation. Useful for when you want to
 * apply variables at execution time rather than at render time.
 *
 * Accepts the following arguments:
 *
 * Mutation - a graphql_ppx or graphql_ppx_re module containing the
 * type t of the GraphQL mutation, the query string of the GraphQL mutation,
 * and a parse function for decoding the JSON response.
 */
module type MutationConfig = {
  type t;
  let query: string;
  let parse: Js.Json.t => t;
};

module type MakeMutationType =
  (Mutation: MutationConfig) =>
   {
    let useMutation:
      unit =>
      (
        UrqlTypes.hookResponse(Mutation.t),
        React.callback(
          option(Js.Json.t),
          Js.Promise.t(UrqlClient.ClientTypes.operationResult),
        ),
      );
  };

module MakeMutation: MakeMutationType =
  (Mutation: MutationConfig) => {
    let useMutation = () => {
      let (responseJs, executeMutationJs) = useMutationJs(Mutation.query);

      let response =
        React.useMemo2(
          () => responseJs |> urqlResponseToReason(Mutation.parse),
          (Mutation.parse, responseJs),
        );

      let executeMutation =
        React.useCallback1(
          variables => executeMutationJs(variables),
          [|executeMutationJs|],
        );

      (response, executeMutation);
    };

    useMutation;
  };
