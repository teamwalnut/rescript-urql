type useQueryArgsJs = {
  query: string,
  variables: Js.Json.t,
  requestPolicy: option(string),
  pollInterval: option(int),
  context: Types.partialOperationContext,
  pause: option(bool),
};

type executeQueryJs = Types.partialOperationContext => unit;

type useQueryResponseJs('extensions) = (
  Types.hookResponseJs(Js.Json.t, 'extensions),
  executeQueryJs,
);

type executeQuery =
  (
    ~additionalTypenames: array(string)=?,
    ~fetchOptions: Fetch.requestInit=?,
    ~fetch: (string, Fetch.requestInit) => Js.Promise.t(Fetch.response)=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~url: string=?,
    ~pollInterval: int=?,
    ~meta: Types.operationDebugMeta=?,
    ~suspense: bool=?,
    ~preferGetMethod: bool=?,
    unit
  ) =>
  unit;

type useQueryResponse('response, 'extensions) = (
  Types.hookResponse('response, 'extensions),
  executeQuery,
);

[@bs.module "urql"]
external useQueryJs: useQueryArgsJs => useQueryResponseJs('extensions) =
  "useQuery";

// reason-react does not provide a binding of sufficient arity for our memoization needs
[@bs.module "react"]
external useMemo9:
  ([@bs.uncurry] (unit => 'any), ('a, 'b, 'c, 'd, 'e, 'f, 'g, 'h, 'i)) => 'any =
  "useMemo";

let useQuery =
    (
      ~request,
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
      (),
    ) => {
  let query = request##query;
  let variables = request##variables;
  let parse = request##parse;
  let rp =
    React.useMemo1(
      () => requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
      [|requestPolicy|],
    );

  let context =
    useMemo9(
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
    );

  let args =
    React.useMemo6(
      () =>
        {query, variables, requestPolicy: rp, pollInterval, pause, context},
      (query, variables, rp, pollInterval, pause, context),
    );

  let (stateJs, executeQueryJs) = useQueryJs(args);

  let state =
    React.useMemo2(
      () => Types.urqlResponseToReason(~response=stateJs, ~parse),
      (stateJs, parse),
    );

  let executeQuery =
    React.useMemo1(
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
        let ctx =
          Types.partialOperationContext(
            ~additionalTypenames?,
            ~fetchOptions?,
            ~fetch?,
            ~url?,
            ~requestPolicy=?
              requestPolicy->Belt.Option.map(Types.requestPolicyToJs),
            ~pollInterval?,
            ~meta?,
            ~suspense?,
            ~preferGetMethod?,
            (),
          );

        executeQueryJs(ctx);
      },
      [|executeQueryJs|],
    );

  (state, executeQuery);
};
