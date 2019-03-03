type t;
type fetchOptions =
  | FetchObj(Fetch.requestInit)
  | FetchFn(unit => Fetch.requestInit);

let unwrapFetchOptions = fetchOptions =>
  switch (fetchOptions) {
  | FetchObj(obj) => obj
  | FetchFn(fn) => fn()
  };

[@bs.deriving abstract]
type clientOptions = {
  url: string,
  [@bs.optional]
  fetchOptions: Fetch.requestInit,
};

[@bs.new] [@bs.module "urql"] external client: clientOptions => t = "Client";

[@bs.send]
external executeQuery:
  (~client: t, ~query: UrqlTypes.graphqlRequest) => Wonka.Types.sourceT('a) =
  "";

[@bs.send]
external executeMutation:
  (~client: t, ~mutation: UrqlTypes.graphqlRequest) => Wonka.Types.sourceT('a) =
  "";
/*
   `make` is equivalent to urql's `createClient`:
   https://github.com/FormidableLabs/urql/blob/cedff86c5a22a20fe659390c6ecdfff22110ff9e/src/client.ts#L43.
   We opt to use `make` here to adhere to standards in the Reason community.
 */
let make =
    (~url: string, ~fetchOptions=FetchObj(Fetch.RequestInit.make()), ()) => {
  let options =
    clientOptions(~url, ~fetchOptions=unwrapFetchOptions(fetchOptions), ());
  client(options);
};