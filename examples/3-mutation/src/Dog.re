open ReasonUrql;
open Hooks;

module Mutations = {
  module LikeDog = [%graphql
    {|
    mutation likeDog($key: ID!) {
      likeDog(key: $key) {
        likes
      }
    }
  |}
  ];

  module PatDog = [%graphql
    {|
    mutation patDog($key: ID!) {
      patDog(key: $key) {
        pats
      }
    }
  |}
  ];

  module TreatDog = [%graphql
    {|
    mutation treatDog($key: ID!) {
      treatDog(key: $key) {
        treats
      }
    }
  |}
  ];

  module BellyscratchDog = [%graphql
    {|
    mutation bellyscratchDog($key: ID!) {
      bellyscratchDog(key: $key) {
        bellyscratches
      }
    }
  |}
  ];
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

  let (_, executeLikeMutation) =
    useMutation(~request=Mutations.LikeDog.make(~key=id, ()));

  module TreatDogMutation = UrqlUseMutation.Make(Mutations.TreatDog);
  let (_, executeTreatMutation) = TreatDogMutation.useMutation();

  <div className=DogStyles.container>
    <img src=imageUrl alt=name className=DogStyles.image />
    <h3 className=DogStyles.title> {j|$name|j}->React.string </h3>
    <div className=DogStyles.buttons>
      <EmojiButton
        emoji={j|👍|j}
        count={string_of_int(likes)}
        hex="48a9dc"
        onClick={_ => executeLikeMutation() |> ignore}
      />
      <Mutation request={Mutations.PatDog.make(~key=id, ())}>
        ...{({executeMutation}) =>
          <EmojiButton
            emoji={j|✋|j}
            count={string_of_int(pats)}
            hex="db4d3f"
            onClick={_ => executeMutation() |> ignore}
          />
        }
      </Mutation>
      <EmojiButton
        emoji={j|🍖|j}
        count={string_of_int(treats)}
        hex="7b16ff"
        onClick={_ => executeTreatMutation(payload) |> ignore}
      />
      <Mutation request={Mutations.BellyscratchDog.make(~key=id, ())}>
        ...{({executeMutation}) =>
          <EmojiButton
            emoji={j|🐾|j}
            count={string_of_int(bellyscratches)}
            hex="1bda2a"
            onClick={_ => executeMutation() |> ignore}
          />
        }
      </Mutation>
    </div>
    <div> description->React.string </div>
  </div>;
};
