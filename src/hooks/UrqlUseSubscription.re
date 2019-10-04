open UrqlTypes;

type handler('acc, 'resp, 'ret) =
  | Handler((option('acc), 'resp) => 'acc): handler('acc, 'resp, 'acc)
  | NoHandler: handler(_, 'resp, 'resp);

[@bs.deriving abstract]
type useSubscriptionArgs = {
  query: string,
  [@bs.optional]
  variables: Js.Json.t,
};

[@bs.deriving abstract]
type useSubscriptionResponseJs('ret) = {
  fetching: bool,
  data: Js.Nullable.t('ret),
  [@bs.optional]
  error: UrqlCombinedError.combinedErrorJs,
};

[@bs.module "urql"]
external useSubscriptionJs:
  (useSubscriptionArgs, option((option('acc), Js.Json.t) => 'acc)) =>
  array(useSubscriptionResponseJs('ret)) =
  "useSubscription";

let useSubscriptionResponseToRecord = (parse, result) => {
  let data = result->dataGet->Js.Nullable.toOption->Belt.Option.map(parse);
  let error =
    result
    ->errorGet
    ->Belt.Option.map(UrqlCombinedError.combinedErrorToRecord);
  let fetching = result->fetchingGet;

  let response =
    switch (fetching, data, error) {
    | (true, None, _) => Fetching
    | (false, _, Some(error)) => Error(error)
    | (true, Some(data), _) => Data(data)
    | (false, Some(data), _) => Data(data)
    | (false, None, None) => NotFound
    };

  {fetching, data, error, response};
};

let useSubscription =
    (
      type acc,
      type resp,
      type ret,
      ~handler: handler(acc, resp, ret),
      ~request: request(resp),
    )
    : hookResponse(ret) => {
  let parse = request##parse;
  let args =
    useSubscriptionArgs(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );

  React.useMemo3(
    () => {
      let state: hookResponse(ret) =
        switch (handler) {
        | NoHandler =>
          useSubscriptionJs(args, None)[0]
          |> useSubscriptionResponseToRecord(parse)
        | Handler(handler_fn) =>
          useSubscriptionJs(
            args,
            Some((acc, data) => handler_fn(acc, parse(data))),
          )[0]
          |> useSubscriptionResponseToRecord(x => x)
        };

      state;
    },
    (handler, args, parse),
  );
};
