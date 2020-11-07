open ReasonUrql;

module LikeDog = [%graphql
  {|
    mutation likeDog($key: ID!) {
      likeDog(key: $key) {
        likes
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

module PatDog = [%graphql
  {|
    mutation patDog($key: ID!) {
      patDog(key: $key) {
        pats
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
  let (likeState, executeLikeMutation) =
    Hooks.useMutation(~query=(module LikeDog));

  let (treatState, executeTreatMutation) =
    Hooks.useMutation(~query=(module TreatDog));

  let (patState, executePatMutation) =
    Hooks.useMutation(~query=(module PatDog));

  let (bellyscratchState, executeBellyscratchMutation) =
    Hooks.useMutation(~query=(module BellyscratchDog));

  <div className="dog">
    <img src=imageUrl alt=name className="dog__image" />
    <h3 className="dog__title"> {j|$name|j}->React.string </h3>
    <div className="dog__buttons">
      {likeState.fetching
       || treatState.fetching
       || patState.fetching
       || bellyscratchState.fetching
         ? <Loading />
         : <>
             <EmojiButton
               emoji={j|👍|j}
               count={string_of_int(likes)}
               className="emoji-button--like"
               onClick={_ => executeLikeMutation({key: id}) |> ignore}
             />
             <EmojiButton
               emoji={j|✋|j}
               count={string_of_int(pats)}
               className="emoji-button--pat"
               onClick={_ => executePatMutation({key: id}) |> ignore}
             />
             <EmojiButton
               emoji={j|🍖|j}
               count={string_of_int(treats)}
               className="emoji-button--treat"
               onClick={_ => executeTreatMutation({key: id}) |> ignore}
             />
             <EmojiButton
               emoji={j|🐾|j}
               count={string_of_int(bellyscratches)}
               className="emoji-button--bellyscratch"
               onClick={_ => executeBellyscratchMutation({key: id}) |> ignore}
             />
           </>}
    </div>
    <p className="dog__description"> description->React.string </p>
  </div>;
};
