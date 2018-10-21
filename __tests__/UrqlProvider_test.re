open Jest;

open ReasonUrql;

describe("UrqlProvider", () => {
  let testClient = Client.make(~url="https://localhost:3000", ());
  test("should render when provided a client instance", () =>
    Expect.(expect(<Provider client=testClient />) |> toMatchSnapshot)
  );
});