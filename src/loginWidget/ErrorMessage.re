[@react.component]
let make = (~message) => {
  let theme = Mui_Theme.useTheme();

  <MaterialUi_FormControl
    fullWidth=true
    className={IdentityDialogStyles.Styles.formElement(theme)}
    error=true>
    <MaterialUi_FormHelperText
      className=Css.(style([textAlign(`center), fontSize(px(16))]))>
      {message |> Js.String.replace("invalid_grant:", "") |> React.string}
    </MaterialUi_FormHelperText>
  </MaterialUi_FormControl>;
};
