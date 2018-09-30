open ReasonUrql;

/* Setup a GraphQL query to get all dogs from our API. */
module GetAllDogs = [%graphql
  {|
query dogs {
  dogs {
    name
    breed
    description
  }
}|}
];
let queryAllDogs = Query.query(GetAllDogs.make());

/* Setup a GraphQL mutation to increment a dog's likes. */
module LikeDog = [%graphql
  {|
  mutation likeDog($key: ID!) {
    likeDog(key: $key) {
      name
      key
      breed
      likes
    }
  }
|}
];

let likeDog = Mutation.mutation(LikeDog.make(~key="VmeRTX7j-", ()));

/* Setup some standard fetch options for the client. */
let makeFetchOptions =
  Fetch.RequestInit.make(
    ~method_=Post,
    ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
    (),
  );

let fetchOptions = Client.FetchObj(makeFetchOptions);

/* Create a custom cache! Our cache will simply store all query data as JSON in
   a local object store. Then, when reading from the cache, we'll simply decode the
   JSON using bs-json. */
let store = Js.Dict.empty();

let write = (~hash, ~data) =>
  Js.Promise.make((~resolve, ~reject) => {
    switch (Js.Json.stringifyAny(data)) {
    | Some(res) => Js.Dict.set(store, hash, res)
    | None => reject(. Js.Exn.raiseError({j|Unable to stringify $data|j}))
    };

    resolve(. (): 'a);
  });
let read = (~hash) =>
  Js.Promise.make((~resolve, ~reject) =>
    switch (Js.Dict.get(store, hash)) {
    | Some(res) =>
      switch (Js.Json.parseExn(res) |> Js.Json.decodeObject) {
      | Some(json) => resolve(. Some(json))
      | None =>
        reject(. Js.Exn.raiseError({j|Could not parse JSON for $hash.|j}))
      }
    | None => resolve(. None)
    }
  );
let invalidate = (~hash: string) =>
  Js.Promise.make((~resolve, ~reject as _) => {
    Js.Dict.unsafeDeleteKey(. store, hash);
    resolve(. (): 'a);
  });
let invalidateAll = () =>
  Js.Promise.make((~resolve, ~reject as _) => {
    Array.iter(
      hash => Js.Dict.unsafeDeleteKey(. store, hash),
      Js.Dict.keys(store),
    );
    resolve(. (): 'a);
  });

let update = (~callback) =>
  Js.Promise.make((~resolve, ~reject as _) => {
    Array.iter(
      hash =>
        switch (Js.Dict.get(store, hash)) {
        | Some(res) =>
          switch (Js.Json.parseExn(res) |> Js.Json.decodeObject) {
          | Some(json) => callback(store, hash, Some(json))
          | None => ()
          }
        | None => ()
        },
      Js.Dict.keys(store),
    );
    resolve(. (): 'a);
  });

let cache: Client.cache(option(Js.Dict.t(Js.Json.t)), Js.Dict.t(string)) = {
  write,
  read,
  invalidate,
  invalidateAll,
  update,
};

/* Set up our client. */
let client =
  Client.make(
    ~url="https://formidadog-ql.now.sh",
    ~cache,
    ~initialCache=store,
    ~fetchOptions,
    (),
  );

/* Let's execute a query and a mutation! */
Client.executeQuery(~client, ~query=queryAllDogs, ~skipCache=false)
|> Js.Promise.then_(value => {
     let dogs = value##data##dogs;
     Js.log2("Dogs", value##data##dogs);
     Js.Promise.resolve(dogs);
   })
|> Js.Promise.catch(err => {
     Js.log2("Something went wrong!", err);
     Js.Promise.resolve(err);
   });

Client.executeMutation(~client, ~mutation=likeDog)
|> Js.Promise.then_(value => {
     let likeDog = value##likeDog;
     Js.log2("Like Dog", likeDog);
     Js.Promise.resolve(likeDog);
   })
|> Js.Promise.catch(err => {
     Js.log2("Something went wrong!", err);
     Js.Promise.resolve(err);
   });

/* Cool, let's render the App now! */
ReactDOMRe.renderToElementWithId(<App client />, "root");