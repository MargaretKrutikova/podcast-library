module Styles = {
  open Css;
  let successNotification =
    style([
      backgroundColor(
        hex(MaterialUi_Color_Green.c700 |> Utils.fromMuiColor),
      ),
    ]);

  let getErrorColor = theme =>
    MaterialUi.Theme.(
      theme->Theme.paletteGet->Palette.errorGet->PaletteColor.mainGet
    );

  let errorNotification = theme =>
    style([
      backgroundColor(hex(theme |> getErrorColor |> Utils.fromMuiColor)),
    ]);

  let snackbarRoot = theme =>
    style([
      paddingLeft(px(theme |> Utils.spacingPx(1))),
      paddingRight(px(theme |> Utils.spacingPx(1))),
    ]);
  let snackbarContent = style([flexGrow(0.0)]);

  let typeToClassName = (type_: AppNotifications.type_, theme) =>
    switch (type_) {
    | Primary => "primary"
    | Secondary => "secondary"
    | Success => successNotification
    | Error => errorNotification(theme)
    | Warning => "warning"
    | Info => "info"
    | Light => "light"
    | Dark => "dark"
    };
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
  let theme = Mui_Theme.useTheme();

  <MaterialUi_Snackbar
    open_
    anchorOrigin={MaterialUi_Snackbar.AnchorOrigin.make(
      ~horizontal=`Right,
      ~vertical=`Top,
    )}
    ?style
    onExited={_ => remove()}
    className={Styles.snackbarRoot(theme)}
    onClose=handleClose
    autoHideDuration={`Int(autoHideDuration)}>
    <MaterialUi_SnackbarContent
      message=children
      classes=[Root(Styles.snackbarContent)]
      className={Styles.typeToClassName(type_, theme)}
      action={
        <MaterialUi_IconButton
          onClick={_ => setOpen(_ => false)} color=`Inherit>
          <ReactFeather.CloseIcon />
        </MaterialUi_IconButton>
      }
    />
  </MaterialUi_Snackbar>;
};
