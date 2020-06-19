type executeQueryJs = Types.partialOperationContext => unit;

type useQueryResponseJs('extensions) = (
  Types.jsHookResponse(Js.Json.t, 'extensions),
  executeQueryJs,
);

type executeQuery =
  (
    ~fetchOptions: Fetch.requestInit=?,
    ~requestPolicy: Types.requestPolicy=?,
    ~url: string=?,
    ~meta: Types.operationDebugMeta=?,
    ~pollInterval: int=?,
    unit
  ) =>
  unit;

type useQueryResponse('response, 'extensions) = (
  Types.hookResponse('response, 'extensions),
  executeQuery,
);

type useQueryArgs = {
  query: string,
  variables: Js.Json.t,
  requestPolicy: option(string),
  pause: option(bool),
  context: Types.partialOperationContext,
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
    UseSemanticGuarantee.useSemanticGuarantee(
      () => Belt.Option.map(requestPolicy, Types.requestPolicyToJs),
      requestPolicy,
    );
  let context =
    React.useMemo4(
      () => {
        Types.partialOperationContext(
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
      () => Types.urqlResponseToReason(~response=stateJs, ~parse),
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
          Types.partialOperationContext(
            ~fetchOptions?,
            ~requestPolicy=?
              Belt.Option.map(requestPolicy, Types.requestPolicyToJs),
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
