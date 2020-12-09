@@ocaml.doc(
  "
* The handler type used to type the optional accumulator function
* returned by useSubscription. handler is a GADT used to support
* proper type inference for useSubscription.
"
)
type rec handler<'acc, 'response, 'ret> =
  | Handler((option<'acc>, 'response) => 'acc): handler<'acc, 'response, 'acc>
  | NoHandler: handler<'response, 'response, 'response>

type useSubscriptionArgs = {
  query: string,
  variables: Js.Json.t,
  pause: option<bool>,
  context: Types.partialOperationContext,
}

type executeSubscriptionJs = Types.partialOperationContext => unit

@module("urql")
external useSubscriptionJs: (
  useSubscriptionArgs,
  option<(option<'acc>, 'dataJs) => 'acc>,
) => (Types.Hooks.hookResponseJs<'ret>, executeSubscriptionJs) = "useSubscription"

type executeSubscription = (
  ~additionalTypenames: array<string>=?,
  ~fetchOptions: Fetch.requestInit=?,
  ~fetch: (string, Fetch.requestInit) => Js.Promise.t<Fetch.response>=?,
  ~requestPolicy: Types.requestPolicy=?,
  ~url: string=?,
  ~pollInterval: int=?,
  ~meta: Types.operationDebugMeta=?,
  ~suspense: bool=?,
  ~preferGetMethod: bool=?,
  unit,
) => unit

type useSubscriptionResponse<'response> = (Types.Hooks.hookResponse<'response>, executeSubscription)

let subscriptionResponseToReason = (response: Types.Hooks.hookResponseJs<'ret>) => {
  let {Types.Hooks.operation: operation, fetching, error, extensions, stale} = response

  let data = response.data->Js.Nullable.toOption

  let response = switch (fetching, data, error) {
  | (true, None, None) => Types.Hooks.Fetching
  | (_, Some(d), None) => Data(d)
  | (_, Some(d), Some(e)) => PartialData(d, e.graphQLErrors)
  | (_, None, Some(e)) => Error(e)
  | (false, None, None) => Empty
  }

  open Types.Hooks
  {
    operation: operation,
    fetching: fetching,
    data: data,
    error: error,
    response: response,
    extensions: extensions,
    stale: stale,
  }
}

// reason-react does not provide a binding of sufficient arity for our memoization needs
@module("react")
external useMemo9: (@uncurry (unit => 'any), ('a, 'b, 'c, 'd, 'e, 'f, 'g, 'h, 'i)) => 'any =
  "useMemo"

let useSubscription = (
  type acc ret variables data,
  ~subscription as module(Subscription: Types.Operation with
    type t = data
    and type t_variables = variables
  ),
  ~handler: handler<acc, data, ret>,
  ~pause=?,
  ~additionalTypenames=?,
  ~fetchOptions=?,
  ~fetch=?,
  ~requestPolicy=?,
  ~url=?,
  ~pollInterval=?,
  ~meta=?,
  ~suspense=?,
  ~preferGetMethod=?,
  variables,
) => {
  let query = Subscription.query
  let parse = Subscription.parse
  let rp = React.useMemo1(
    () => requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
    [requestPolicy],
  )

  let context = useMemo9(
    () =>
      Types.partialOperationContext(
        ~additionalTypenames?,
        ~fetchOptions?,
        ~fetch?,
        ~url?,
        ~requestPolicy=?rp,
        ~pollInterval?,
        ~meta?,
        ~suspense?,
        ~preferGetMethod?,
        (),
      ),
    (
      additionalTypenames,
      fetchOptions,
      fetch,
      url,
      rp,
      pollInterval,
      meta,
      suspense,
      preferGetMethod,
    ),
  )

  let args = React.useMemo4(() => {
    query: query,
    variables: {
      open Subscription
      variables->serializeVariables->variablesToJson
    },
    pause: pause,
    context: context,
  }, (query, variables, pause, context))

  let h = (acc, subscriptionResult) =>
    switch handler {
    | Handler(handlerFn) => handlerFn(acc, parse(subscriptionResult))
    | NoHandler => parse(subscriptionResult)
    }

  let (responseJs, executeSubscriptionJs) = useSubscriptionJs(args, Some(h))

  let state = React.useMemo1(() => subscriptionResponseToReason(responseJs), [responseJs])

  let executeSubscription = React.useMemo1(
    (
      (),
      ~additionalTypenames=?,
      ~fetchOptions=?,
      ~fetch=?,
      ~requestPolicy=?,
      ~url=?,
      ~pollInterval=?,
      ~meta=?,
      ~suspense=?,
      ~preferGetMethod=?,
      (),
    ) => {
      let ctx = Types.partialOperationContext(
        ~additionalTypenames?,
        ~fetchOptions?,
        ~fetch?,
        ~url?,
        ~requestPolicy=?requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
        ~pollInterval?,
        ~meta?,
        ~suspense?,
        ~preferGetMethod?,
        (),
      )

      executeSubscriptionJs(ctx)
    },
    [executeSubscriptionJs],
  )

  (state, executeSubscription)
}
