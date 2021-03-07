type useQueryArgsJs = {
  query: string,
  variables: Js.Json.t,
  requestPolicy: option<string>,
  context: Types.partialOperationContext,
  pause: option<bool>,
}

type executeQueryJs = Types.partialOperationContext => unit

type useQueryResponseJs<'dataJs> = (Types.Hooks.hookResponseJs<'dataJs>, executeQueryJs)

type executeQuery = (
  ~additionalTypenames: array<string>=?,
  ~fetchOptions: Fetch.requestInit=?,
  ~fetch: (string, Fetch.requestInit) => Js.Promise.t<Fetch.response>=?,
  ~requestPolicy: Types.requestPolicy=?,
  ~url: string=?,
  ~meta: Types.operationDebugMeta=?,
  ~suspense: bool=?,
  ~preferGetMethod: bool=?,
  unit,
) => unit

type useQueryResponse<'data> = (Types.Hooks.hookResponse<'data>, executeQuery)

@module("urql")
external useQueryJs: useQueryArgsJs => useQueryResponseJs<'dataJs> = "useQuery"

// @rescript/react does not provide a binding of sufficient arity for our memoization needs.
@module("react")
external useMemo8: (@uncurry (unit => 'any), ('a, 'b, 'c, 'd, 'e, 'f, 'g, 'h)) => 'any = "useMemo"

let useQuery:
  type variables variablesJs data. (
    ~query: module(Types.Operation with
      type t_variables = variables
      and type Raw.t_variables = variablesJs
      and type t = data
    ),
    ~pause: bool=?,
    ~additionalTypenames: array<string>=?,
    ~fetchOptions: Fetch.requestInit=?,
    ~fetch: (string, Fetch.requestInit) => Js.Promise.t<Fetch.response>=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~url: string=?,
    ~meta: Types.operationDebugMeta=?,
    ~suspense: bool=?,
    ~preferGetMethod: bool=?,
    variables,
  ) => useQueryResponse<data> =
  (
    ~query as module(Query),
    ~pause=?,
    ~additionalTypenames=?,
    ~fetchOptions=?,
    ~fetch=?,
    ~requestPolicy=?,
    ~url=?,
    ~meta=?,
    ~suspense=?,
    ~preferGetMethod=?,
    variables,
  ) => {
    let query = Query.query
    let parse = Query.parse
    let rp = React.useMemo1(
      () => requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
      [requestPolicy],
    )

    let context = useMemo8(
      () =>
        Types.partialOperationContext(
          ~additionalTypenames?,
          ~fetchOptions?,
          ~fetch?,
          ~url?,
          ~requestPolicy=?rp,
          ~meta?,
          ~suspense?,
          ~preferGetMethod?,
          (),
        ),
      (additionalTypenames, fetchOptions, fetch, url, rp, meta, suspense, preferGetMethod),
    )

    let args = React.useMemo5(() => {
      query: query,
      variables: {
        open Query
        variables->serializeVariables->variablesToJson
      },
      requestPolicy: rp,
      pause: pause,
      context: context,
    }, (query, variables, rp, pause, context))

    let (stateJs, executeQueryJs) = useQueryJs(args)

    let state = React.useMemo2(
      () => Types.Hooks.hookResponseToReason(~response=stateJs, ~parse),
      (stateJs, parse),
    )

    let executeQuery = React.useMemo1(
      (
        (),
        ~additionalTypenames=?,
        ~fetchOptions=?,
        ~fetch=?,
        ~requestPolicy=?,
        ~url=?,
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
          ~meta?,
          ~suspense?,
          ~preferGetMethod?,
          (),
        )

        executeQueryJs(ctx)
      },
      [executeQueryJs],
    )

    (state, executeQuery)
  }
