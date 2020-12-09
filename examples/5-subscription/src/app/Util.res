@scope("Math") @val external random: unit => float = "random"
@scope("Math") @val external floor: float => int = "floor"
@send external toString: (int, int) => string = "toString"

let getRandomInt = (max: int) => floor(random() *. float_of_int(max))

let getRandomHex = () => {
  let encode = random() *. float_of_int(16777215) |> floor
  let hex = encode->toString(16)
  j`#$hex`
}
