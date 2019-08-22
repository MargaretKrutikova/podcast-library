let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "ContentTabsStyles"(theme =>
    {
      contentTabs: style(~marginBottom=theme |> Utils.spacing(2), ()),
      contentTab:
        style(~flex="1 1 0", ~maxWidth="none", ())
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=style(~flex="0", ()),
          ),
    }
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
    centered=true>
    <MaterialUi_Tab
      onClick={_ => onTabChange(ContentType.Episode)}
      label={React.string("Episodes")}
      className={classes.contentTab}
    />
    <MaterialUi_Tab
      onClick={_ => onTabChange(ContentType.Podcast)}
      label={React.string("Podcasts")}
      className={classes.contentTab}
    />
  </MaterialUi_Tabs>;
};
