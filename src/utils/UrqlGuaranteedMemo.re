// React.useMemo has no semantic guarantee so we define our own
let useGuaranteedMemo1 = (computeValue, arg) => {
  let (memoizedState, setMemoizedState) =
    React.useState(() => computeValue(arg));

  React.useEffect1(
    () => {
      setMemoizedState(_ => computeValue(arg));
      None;
    },
    [|arg|],
  );

  memoizedState;
};
