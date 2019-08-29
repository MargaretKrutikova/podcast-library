open AppStyles;

let typeToClassName = (type_: AppNotifications.type_, classes) =>
  switch (type_) {
  | Primary => "primary"
  | Secondary => "secondary"
  | Success => classes.successNotification
  | Error => classes.errorNotification
  | Warning => "warning"
  | Info => "info"
  | Light => "light"
  | Dark => "dark"
  };

let autoHideDuration = 1500;

[@bs.deriving abstract]
type transitionProps = {timeout: int};

[@react.component]
let make = (~type_, ~id, ~style=?, ~children) => {
  let dispatch = AppCore.useDispatch();
  let remove = () => dispatch(RemoveNotification(id));

  let (open_, setOpen) = React.useState(() => true);
  let handleClose = (_, reason) =>
    if (reason !== "clickaway") {
      setOpen(_ => false);
    };
  let classes = AppStyles.useStyles();

  <MaterialUi_Snackbar
    open_
    anchorOrigin={MaterialUi_Snackbar.AnchorOrigin.make(
      ~horizontal=`Right,
      ~vertical=`Top,
    )}
    ?style
    onExited={_ => remove()}
    className={classes.snackbarRoot}
    onClose=handleClose
    autoHideDuration={`Int(autoHideDuration)}>
    <MaterialUi_SnackbarContent
      message=children
      classes=[
        Root(
          typeToClassName(type_, classes) ++ " " ++ classes.snackbarContent,
        ),
      ]
      className={typeToClassName(type_, classes)}
      action={
        <MaterialUi_IconButton
          onClick={_ => setOpen(_ => false)} color=`Inherit>
          <ReactFeather.CloseIcon />
        </MaterialUi_IconButton>
      }
    />
  </MaterialUi_Snackbar>;
};
