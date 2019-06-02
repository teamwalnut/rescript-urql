open ReasonUrql;

module Mutations = {
  let likeDog = {|
    mutation likeDog($key: ID!) {
      likeDog(key: $key) {
        likes
      }
    }
  |};

  let patDog = {|
    mutation patDog($key: ID!) {
      patDog(key: $key) {
        pats
      }
    }
  |};

  let treatDog = {|
    mutation treatDog($key: ID!) {
      treatDog(key: $key) {
        treats
      }
    }
  |};

  let bellyscratchDog = {|
    mutation bellyscratchDog($key: ID!) {
      bellyscratchDog(key: $key) {
        bellyscratches
      }
    }
  |};
};

[@react.component]
let make =
    (
      ~id: string,
      ~name: string,
      ~imageUrl: string,
      ~likes: int,
      ~pats: int,
      ~treats: int,
      ~bellyscratches: int,
      ~description: string,
    ) => {
  let payload =
    React.useMemo1(
      () => {
        let variables = Js.Dict.empty();
        Js.Dict.set(variables, "key", Js.Json.string(id));
        Js.Json.object_(variables);
      },
      [|id|],
    );

  let (_, executeMutation) = Hooks.useMutation(~query=Mutations.treatDog);

  <div className=DogStyles.container>
    <img src=imageUrl alt=name className=DogStyles.image />
    <h3 className=DogStyles.title> {j|$name|j}->React.string </h3>
    <div className=DogStyles.buttons>
      <Mutation query=Mutations.likeDog>
        ...{({executeMutation}) =>
          <EmojiButton
            emoji={j|👍|j}
            count={string_of_int(likes)}
            hex="48a9dc"
            onClick={_ => executeMutation(Some(payload)) |> ignore}
          />
        }
      </Mutation>
      <Mutation query=Mutations.patDog>
        ...{({executeMutation}) =>
          <EmojiButton
            emoji={j|✋|j}
            count={string_of_int(pats)}
            hex="db4d3f"
            onClick={_ => executeMutation(Some(payload)) |> ignore}
          />
        }
      </Mutation>
      <EmojiButton
        emoji={j|🍖|j}
        count={string_of_int(treats)}
        hex="7b16ff"
        onClick={_ => executeMutation(Some(payload)) |> ignore}
      />
      <Mutation query=Mutations.bellyscratchDog>
        ...{({executeMutation}) =>
          <EmojiButton
            emoji={j|🐾|j}
            count={string_of_int(bellyscratches)}
            hex="1bda2a"
            onClick={_ => executeMutation(Some(payload)) |> ignore}
          />
        }
      </Mutation>
    </div>
    <div> description->React.string </div>
  </div>;
};