[@react.component]
let make = (~onClick, ~children) => {
  let classes = IdentityDialogStyles.IdentityDialogStyles.useStyles();

  <MaterialUi_Button
    onClick
    color=`Primary
    fullWidth=true
    component={`String("button")}
    classes=[TextPrimary(classes.userLink)]>
    children
  </MaterialUi_Button>;
};
