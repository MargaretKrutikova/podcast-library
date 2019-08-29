[@react.component]
let make = (~title) => {
  let classes = AppStyles.AppStyles.useStyles();
  
  <MaterialUi_Typography
    className={classes.pageTitle}
    variant=`H3
    component={`String("h1")}
    gutterBottom=true>
    {React.string(title)}
  </MaterialUi_Typography>;
};
