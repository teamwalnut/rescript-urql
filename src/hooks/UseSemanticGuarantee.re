open React.Ref;

let useSemanticGuarantee = (compute, dep) => {
  let isFirstRun = React.useRef(true);
  let value = React.useRef(isFirstRun->current ? compute() : None);
  let prevDep = React.useRef(dep);

  isFirstRun->setCurrent(false);

  let isEqual = dep === prevDep->current;

  if (!isEqual) {
    prevDep->setCurrent(dep);
    value->setCurrent(compute());
  };

  value->current;
};
