include UseClient;
include UseQuery;
include UseMutation;
include UseSubscription;

type hookResponse('ret, 'extensions) =
  Types.hookResponse('ret, 'extensions) = {
    fetching: bool,
    data: option('ret),
    error: option(CombinedError.t),
    response: Types.response('ret),
    extensions: option('extensions),
  };
