let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "CardActionsStyles"(theme =>
    {
      root:
        style(
          ~padding=theme |> Utils.spacing(2),
          ~paddingTop="0px",
          ~justifyContent="flex-end",
          (),
        ),
    }
  )
];

module CardActions = {
  [@react.component]
  let make = (~children) => {
    let classes = CardActionsStyles.useStyles();
    <MaterialUi_CardActions className={classes.root}>
      children
    </MaterialUi_CardActions>;
  };
};

[%mui.withStyles
  "SearchCardStyles"(theme =>
    {
      card: style(~position="relative", ~overflow="initial", ()),
      savedIcon:
        style(
          ~position="absolute",
          ~right=theme |> Utils.spacing(1),
          ~top=theme |> Utils.spacingTransform(1, space => space * (-1)),
          (),
        ),
      searchInput: style(~fontSize="24px", ()),
    }
  )
];

module SearchCard = {
  [@react.component]
  let make = (~isSaved, ~children) => {
    let classes = SearchCardStyles.useStyles();

    <MaterialUi_Card className={classes.card}>
      {isSaved
         ? <ReactFeather.BookmarkIcon
             className={classes.savedIcon}
             fill="#669BBC"
             color="#29335C"
           />
         : React.null}
      children
    </MaterialUi_Card>;
  };
};

let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "CardDescriptionStyles"(_ => {root: style(~overflow="hidden", ())})
];

module Description = {
  [@react.component]
  let make = (~description) => {
    let classes = CardDescriptionStyles.useStyles();

    <MaterialUi_Typography
      variant=`Body1
      color=`TextSecondary
      component={`String("div")}
      className={classes.root}>
      <div
        dangerouslySetInnerHTML={
          "__html": Utils.truncateDescription(description),
        }
      />
    </MaterialUi_Typography>;
  };
};
