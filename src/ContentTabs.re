[@react.component]
let make = (~activeTab, ~onTabChange) => {
  let theme = Mui_Theme.useTheme();
  let contentTabCss =
    Css.(
      style([
        flexGrow(1.0),
        flexShrink(1.0),
        flexBasis(`zero),
        maxWidth(`none),
        media(Utils.getBreakpoint(`MD, theme), [flex(`num(0.0))]),
      ])
    );

  <MaterialUi_Tabs
    className=Css.(style([marginBottom(px(theme |> Utils.spacingPx(2)))]))
    value={activeTab === ContentType.Episode ? 0 : 1}
    indicatorColor=`Primary
    textColor=`Primary
    centered=true>
    <MaterialUi_Tab
      onClick={_ => onTabChange(ContentType.Episode)}
      label={React.string("Episodes")}
      className=contentTabCss
    />
    <MaterialUi_Tab
      onClick={_ => onTabChange(ContentType.Podcast)}
      label={React.string("Podcasts")}
      className=contentTabCss
    />
  </MaterialUi_Tabs>;
};
