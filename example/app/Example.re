open ReasonUrql;

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

module GetDog = [%graphql
  {|
query dog($key: ID!) {
  dog(key: $key) {
    name
    breed
  }
}
|}
];
let queryOneDog = Query.query(GetDog.make(~key="VmeRTX7j-", ()));
Js.log2("queryOneDog", queryOneDog);

module QuerySolo = Query.Make(GetDog);
let queryFinal =
  QuerySolo.queryFn(
    ~variables=Json.Encode.(object_([("key", string("VmeRTX7j-"))])),
    (),
  );
Js.log2("queryFinal", queryFinal);

let makeFetchOptions =
  Fetch.RequestInit.make(
    ~method_=Post,
    ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
    (),
  );

let fetchOptions = Client.FetchObj(makeFetchOptions);

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

let client =
  Client.make(~url="https://formidadog-ql.now.sh", ~cache, ~fetchOptions, ());

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

Client.executeQuery(~client, ~query=queryOneDog, ~skipCache=false)
|> Js.Promise.then_(value => {
     let dog = value##data##dog;
     Js.log2("Dog", dog);
     Js.Promise.resolve(dog);
   })
|> Js.Promise.catch(err => {
     Js.log2("Something went wrong!", err);
     Js.Promise.resolve(err);
   });

/* Cool, let's render the App now! */
ReactDOMRe.renderToElementWithId(<App client />, "root");