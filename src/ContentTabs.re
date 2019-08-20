let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "ContentTabsStyles"(theme =>
    {contentTabs: style(~marginBottom=theme |> Utils.spacing(2), ())}
  )
];

[@react.component]
let make = (~activeTab, ~onTabChange) => {
  let classes = ContentTabsStyles.useStyles();

  <MaterialUi_Tabs
    className={classes.contentTabs}
    value={activeTab === ContentType.Episode ? 0 : 1}
    indicatorColor=`Primary
    textColor=`Primary
    variant=`FullWidth>
    <MaterialUi_Tab
      onClick={_ => onTabChange(ContentType.Episode)}
      label={React.string("Episodes")}
    />
    <MaterialUi_Tab
      onClick={_ => onTabChange(ContentType.Podcast)}
      label={React.string("Podcasts")}
    />
  </MaterialUi_Tabs>;
};
