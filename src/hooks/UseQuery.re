type executeQueryJs = Types.partialOperationContext => unit;

type useQueryResponseJs('extensions) = (
  Types.jsHookResponse(Js.Json.t, 'extensions),
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
    UseSemanticGuarantee.useSemanticGuarantee(
      () => Belt.Option.map(requestPolicy, Types.requestPolicyToJs),
      requestPolicy,
    );
  let context = {
    Types.partialOperationContext(
      ~additionalTypenames?,
      ~fetchOptions?,
      ~fetch?,
      ~url?,
      ~pollInterval?,
      ~meta?,
      ~suspense?,
      ~preferGetMethod?,
      (),
    );
  };

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
            ~requestPolicy=?
              Belt.Option.map(requestPolicy, Types.requestPolicyToJs),
            ~url?,
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
