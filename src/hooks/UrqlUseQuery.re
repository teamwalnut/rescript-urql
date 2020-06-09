type executeQueryJs = UrqlTypes.partialOperationContext => unit;

type useQueryResponseJs('extensions) = (
  UrqlTypes.jsHookResponse(Js.Json.t, 'extensions),
  executeQueryJs,
);

type executeQuery =
  (
    ~fetchOptions: Fetch.requestInit=?,
    ~requestPolicy: UrqlTypes.requestPolicy=?,
    ~url: string=?,
    ~meta: UrqlTypes.operationDebugMeta=?,
    ~pollInterval: int=?,
    unit
  ) =>
  unit;

type useQueryResponse('response, 'extensions) = (
  UrqlTypes.hookResponse('response, 'extensions),
  executeQuery,
);

type useQueryArgs = {
  query: string,
  variables: Js.Json.t,
  requestPolicy: option(string),
  pause: option(bool),
  context: UrqlTypes.partialOperationContext,
};

[@bs.module "urql"]
external useQueryJs: useQueryArgs => useQueryResponseJs('extensions) =
  "useQuery";

let useQuery =
    (
      ~request,
      ~pause=?,
      ~fetchOptions=?,
      ~requestPolicy=?,
      ~url=?,
      ~meta=?,
      ~pollInterval=?,
      (),
    ) => {
  let query = request##query;
  let variables = request##variables;
  let parse = request##parse;
  let rp =
    UrqlUseSemanticGuarantee.useSemanticGuarantee(
      () => Belt.Option.map(requestPolicy, UrqlTypes.requestPolicyToJs),
      requestPolicy,
    );
  let context =
    React.useMemo4(
      () => {
        UrqlTypes.partialOperationContext(
          ~fetchOptions?,
          ~url?,
          ~meta?,
          ~pollInterval?,
          (),
        )
      },
      (fetchOptions, url, meta, pollInterval),
    );

  let args = {query, variables, requestPolicy: rp, pause, context};

  let (stateJs, executeQueryJs) = useQueryJs(args);

  let state =
    React.useMemo2(
      () => UrqlTypes.urqlResponseToReason(~response=stateJs, ~parse),
      (stateJs, parse),
    );

  let executeQuery =
    React.useMemo1(
      (
        (),
        ~fetchOptions=?,
        ~requestPolicy=?,
        ~url=?,
        ~meta=?,
        ~pollInterval=?,
        (),
      ) => {
        let ctx =
          UrqlTypes.partialOperationContext(
            ~fetchOptions?,
            ~requestPolicy=?
              Belt.Option.map(requestPolicy, UrqlTypes.requestPolicyToJs),
            ~url?,
            ~meta?,
            ~pollInterval?,
            (),
          );
        executeQueryJs(ctx);
      },
      [|executeQueryJs|],
    );

  (state, executeQuery);
};
