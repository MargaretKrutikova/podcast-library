open AppStyles;

module CardActions = {
  [@react.component]
  let make = (~children) => {
    let classes = AppStyles.useStyles();
    <MaterialUi_CardActions className={classes.cardActions}>
      children
    </MaterialUi_CardActions>;
  };
};

module SearchCard = {
  [@react.component]
  let make = (~isSaved, ~children) => {
    let classes = AppStyles.useStyles();

    <MaterialUi_Card className={classes.searchCard}>
      {isSaved
         ? <ReactFeather.BookmarkIcon
             className={classes.cardSavedIcon}
             fill="#669BBC"
             color="#29335C"
           />
         : React.null}
      children
    </MaterialUi_Card>;
  };
};

module Description = {
  [@react.component]
  let make = (~description) => {
    let classes = AppStyles.useStyles();

    <MaterialUi_Typography
      variant=`Body1
      color=`TextSecondary
      component={`String("div")}
      className={classes.cardDescription}>
      <div
        dangerouslySetInnerHTML={
          "__html": Utils.truncateDescription(description),
        }
      />
    </MaterialUi_Typography>;
  };
};
