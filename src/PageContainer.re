[@react.component]
let make = (~children) => {
  let classes = AppStyles.AppStyles.useStyles();

  <div className={classes.pageContainer}> children </div>;
};
