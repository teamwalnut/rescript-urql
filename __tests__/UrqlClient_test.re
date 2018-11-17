open Jest;
open ReasonUrql;

describe("UrqlClient", () => {
  describe("Client with only a url provided", () => {
    let client = Client.make(~url="https://localhost:3000", ());

    test("should instantiate a client instance", () =>
      Expect.(expect(client) |> toMatchSnapshot)
    );

    test("should apply the provided url to urql's new Client constructor", () =>
      Expect.(
        expect(
          Client.clientConfig(~url="https://localhost:3000", ())
          ->Client.urlGet,
        )
        |> toEqual("https://localhost:3000")
      )
    );

    test("should expose an executeQuery operation", () =>
      ExpectJs.(expect(Client.executeQuery) |> toBeTruthy)
    );

    test("should expose an executeMutation operation", () =>
      ExpectJs.(expect(Client.executeMutation) |> toBeTruthy)
    );
  });

  describe("Client with fetchOptions provided", () => {
    let fetchOptions =
      Fetch.RequestInit.make(
        ~method_=Post,
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        (),
      );

    let makeFetchOptions = Client.FetchObj(fetchOptions);

    let client =
      Client.make(
        ~url="https://localhost:3000",
        ~fetchOptions=makeFetchOptions,
        (),
      );

    test("should instantiate a client instance with fetchOptions", () =>
      Expect.(expect(client) |> toMatchSnapshot)
    );

    test(
      "should apply the provided fetchOptions to urql's new Client constructor",
      () =>
      Expect.(
        expect(
          Client.clientConfig(
            ~url="https://localhost:3000",
            ~fetchOptions,
            (),
          )
          ->Client.fetchOptionsGet,
        )
        |> toEqual(
             Some(
               Fetch.RequestInit.make(
                 ~method_=Post,
                 ~headers=
                   Fetch.HeadersInit.make({
                     "Content-Type": "application/json",
                   }),
                 (),
               ),
             ),
           )
      )
    );
  });

  describe("Client with initialCache provided", () => {
    let initialCache = Js.Dict.empty();
    Js.Dict.set(
      initialCache,
      "12345",
      "{ name: Dixie, likes: 1000, breed: Pit Bull }",
    );

    let client =
      Client.make(~url="https://localhost:3000", ~initialCache, ());

    test("should instantiate a client instance with initialCache", () =>
      Expect.(expect(client) |> toMatchSnapshot)
    );

    test(
      "should apply the provided initialCache to urql's new Client constructor",
      () =>
      Expect.(
        expect(
          Client.clientConfig(
            ~url="https://localhost:3000",
            ~initialCache,
            (),
          )
          ->Client.initialCacheGet,
        )
        |> toEqual(Some(initialCache))
      )
    );
  });

  describe("Client with cache provided", () => {
    let store = Js.Dict.empty();

    /* Mock out cache functions with noops for brevity. */
    let write = (~hash, ~data) =>
      Js.Promise.make((~resolve, ~reject) => resolve(. (): 'a));

    let read = (~hash) =>
      Js.Promise.make((~resolve, ~reject) => resolve(. "read"));

    let invalidate = (~hash: string) =>
      Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a));

    let invalidateAll = () =>
      Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a));

    let update = (~callback) =>
      Js.Promise.make((~resolve, ~reject as _) => resolve(. (): 'a));

    let cache: Client.cache(string, Js.Dict.t(string)) = {
      write,
      read,
      invalidate,
      invalidateAll,
      update,
    };

    let client = Client.make(~url="https://localhost:3000", ~cache, ());

    test("should instantiate a client instance with cache", () =>
      Expect.(expect(client) |> toMatchSnapshot)
    );
  });
});