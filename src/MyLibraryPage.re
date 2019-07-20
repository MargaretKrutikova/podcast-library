let str = ReasonReact.string;

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let myLibrary = AppCore.useSelector(model => model.myLibrary);

  <div> {str("My library")} </div>;
};
