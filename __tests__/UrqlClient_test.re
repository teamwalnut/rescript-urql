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
});