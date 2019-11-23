open ReasonUrql;

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

  let treatDog = {|
    mutation treatDog($key: ID!) {
      treatDog(key: $key) {
        treats
      }
    }
  |};

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

  // Example of using hooks with graphql_ppx_re (or graphql_ppx).
  let (_, executeLikeMutation) =
    Hooks.useMutation(~request=Mutations.LikeDog.make(~key=id, ()));

  // Example of using hooks without graphql_ppx_re (or graphql_ppx).
  let (_, executeTreatMutation) =
    Hooks.useMutation(
      ~request={
        "query": Mutations.treatDog,
        "variables": payload,
        "parse": x => x,
      },
    );

  /* Example of using hooks where the variables are only known when the
     mutation runs. */
  let (_, executePatMutation) =
    Hooks.useDynamicMutation(
      ~query=Mutations.PatDog.query,
      ~parse=Mutations.PatDog.parse,
    );

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
      <EmojiButton
        emoji={j|✋|j}
        count={string_of_int(pats)}
        hex="db4d3f"
        onClick={_ =>
          executePatMutation(Mutations.PatDog.make(~key=id, ())) |> ignore
        }
      />
      <EmojiButton
        emoji={j|🍖|j}
        count={string_of_int(treats)}
        hex="7b16ff"
        onClick={_ => executeTreatMutation() |> ignore}
      />
      // Example of using the Mutation component.
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
