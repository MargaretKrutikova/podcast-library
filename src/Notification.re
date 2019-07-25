open BsReactstrap;

let typeToColor = (type_: AppNotifications.type_) =>
  switch (type_) {
  | Primary => "primary"
  | Secondary => "secondary"
  | Success => "success"
  | Danger => "danger"
  | Warning => "warning"
  | Info => "info"
  | Light => "light"
  | Dark => "dark"
  };

let fadeMs = 150;

[@bs.deriving abstract]
type transitionProps = {timeout: int};

[@react.component]
let make = (~type_, ~id, ~isShown: bool, ~children) => {
  let dispatch = AppCore.useDispatch();
  let hide = () => dispatch(HideNotification(id));
  let remove = () => dispatch(RemoveNotification(id));

  React.useEffect0(() => {
    let timer = Js.Global.setTimeout(hide, 3000);
    Some(() => Js.Global.clearTimeout(timer));
  });

  React.useEffect1(
    () =>
      if (!isShown) {
        let timer = Js.Global.setTimeout(remove, fadeMs);
        Some(() => Js.Global.clearTimeout(timer));
      } else {
        None;
      },
    [|isShown|],
  );
  <Alert
    color={typeToColor(type_)}
    transition={transitionProps(~timeout=fadeMs)}
    isOpen=isShown
    toggle=hide>
    children
  </Alert>;
};
