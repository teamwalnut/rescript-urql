let urqlResponseToReason:
  (
    ~response: UrqlTypes.jsHookResponse(Js.Json.t, 'extensions),
    ~parse: Js.Json.t => 'response
  ) =>
  UrqlTypes.hookResponse('response, 'extensions);
