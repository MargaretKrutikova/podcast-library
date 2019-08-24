let useStateAsync = (initState: unit => 'a) => {
  let (state, setState) = React.useState(initState);
  let mounted = React.useRef(false);

  React.useEffect0(() => {
    mounted->React.Ref.setCurrent(true);
    Some(() => mounted->React.Ref.setCurrent(false));
  });

  let setStateSafe = updateState =>
    if (mounted->React.Ref.current) {
      setState(updateState);
    };
  (state, setStateSafe);
};
