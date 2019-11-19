/**
 * The handler type used to type the optional accumulator function
 * returned by useSubscription. handler is a GADT used to support
 * proper type inference for useSubscription.
 */
type handler('acc, 'resp, 'ret) =
  | Handler((option('acc), 'resp) => 'acc): handler('acc, 'resp, 'acc)
  | NoHandler: handler(_, 'resp, 'resp);

/* Arguments passed to useSubscription on the JavaScript side. */
[@bs.deriving abstract]
type useSubscriptionArgs = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
};

[@bs.module "urql"]
external useSubscriptionJs:
  (useSubscriptionArgs, option((option('acc), Js.Json.t) => 'acc)) =>
  array(UrqlTypes.jsResponse('ret)) =
  "useSubscription";

/**
 * A function for converting the response to useQuery from the JavaScript
 * representation to a typed Reason record.
 */
let useSubscriptionResponseToRecord =
    (parse, result): UrqlTypes.hookResponse('response) => {
  let data =
    result->UrqlTypes.jsDataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->UrqlTypes.jsErrorGet
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->UrqlTypes.fetchingGet;

  let response =
    switch (fetching, data, error) {
    | (true, None, _) => UrqlTypes.Fetching
    | (false, _, Some(error)) => Error(error)
    | (true, Some(data), _) => Data(data)
    | (false, Some(data), _) => Data(data)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

/**
 * The useSubscription hook.
 *
 * Accepts the following arguments:
 *
 * request – a Js.t containing the query and variables corresponding
 * to the GraphQL subscription, and a parse function for decoding the JSON response.
 *
 * handler – an optional function to accumulate subscription responses.
 */;
let useSubscription =
    (
      type acc,
      type resp,
      type ret,
      ~request: UrqlTypes.request(resp),
      ~handler: handler(acc, resp, ret),
    )
    : UrqlTypes.hookResponse(ret) => {
  let parse = request##parse;

  let args =
    useSubscriptionArgs(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );

  React.useMemo3(
    () => {
      let response: UrqlTypes.hookResponse(ret) =
        switch (handler) {
        | Handler(handlerFn) =>
          useSubscriptionJs(
            args,
            Some((acc, data) => handlerFn(acc, parse(data))),
          )[0]
          |> useSubscriptionResponseToRecord(x => x)
        | NoHandler =>
          useSubscriptionJs(args, None)[0]
          |> useSubscriptionResponseToRecord(parse)
        };

      response;
    },
    (handler, args, parse),
  );
};

/**
 * The functor implementation of useQuery. An alternative to the function API.
 *
 * Accepts the following arguments:
 *
 * Query - a graphql_ppx or graphql_ppx_re module containing the
 * type t of the GraphQL query, the query string of the GraphQL query,
 * and a parse function for decoding the JSON response.
 */
module type SubscriptionConfig = {
  type t;
  let query: string;
  let parse: Js.Json.t => t;
};

module type MakeSubscriptionType = {
  type resp;

  let useSubscription:
    (~variables: Js.Json.t=?, ~handler: handler('acc, resp, 'ret)) =>
    UrqlTypes.hookResponse('ret);
};

module MakeSubscription =
       (Subscription: SubscriptionConfig)
       : (MakeSubscriptionType with type resp = Subscription.t) => {
  type resp = Subscription.t;

  let useSubscription =
      (type acc, type ret, ~variables=?, ~handler: handler(acc, resp, ret))
      : UrqlTypes.hookResponse(ret) => {
    let args =
      useSubscriptionArgs(~query=Subscription.query, ~variables?, ());

    React.useMemo3(
      () => {
        let response: UrqlTypes.hookResponse(ret) =
          switch (handler) {
          | Handler(handlerFn) =>
            useSubscriptionJs(
              args,
              Some(
                (acc, data) => handlerFn(acc, Subscription.parse(data)),
              ),
            )[0]
            |> useSubscriptionResponseToRecord(x => x)
          | NoHandler =>
            useSubscriptionJs(args, None)[0]
            |> useSubscriptionResponseToRecord(Subscription.parse)
          };

        response;
      },
      (handler, args, Subscription.parse),
    );
  };

  useSubscription;
};
