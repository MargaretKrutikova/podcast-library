[@react.component]
let make = (~message) => {
  let classes = IdentityDialogStyles.IdentityDialogStyles.useStyles();

  <MaterialUi_FormControl
    fullWidth=true className={classes.formElement} error=true>
    <MaterialUi_FormHelperText className={classes.errorMsg}>
      {message |> Js.String.replace("invalid_grant:", "") |> React.string}
    </MaterialUi_FormHelperText>
  </MaterialUi_FormControl>;
};
