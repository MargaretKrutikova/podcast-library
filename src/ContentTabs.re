[@react.component]
let make = (~activeTab, ~onTabChange) => {
  let classes = AppStyles.AppStyles.useStyles();

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
