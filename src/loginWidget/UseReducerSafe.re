let useReducerSafe = (reducer, initState) => {
  let (state, dispatch) = React.useReducer(reducer, initState);
  let mounted = React.useRef(false);

  React.useEffect0(() => {
    mounted->React.Ref.setCurrent(true);
    Some(() => mounted->React.Ref.setCurrent(false));
  });

  let setStateSafe = (action: 'a) =>
    if (mounted->React.Ref.current) {
      dispatch(action);
    };
  (state, setStateSafe);
};
