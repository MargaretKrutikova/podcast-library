let str = ReasonReact.string;

module Styles = {
  open Css;

  let searchContainer = theme =>
    style([
      marginBottom(px(theme |> Utils.spacingPx(3))),
      display(`flex),
      flexDirection(`row),
    ]);

  let searchFormControl =
    style([maxWidth(px(600)), margin2(~v=zero, ~h=auto)]);
  let searchInput = style([fontSize(px(26))]);
};

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let searchType = AppCore.useSelector(Selectors.getSearchType);
  let searchPodcast = AppCore.useSelector(Selectors.getSearchPodcast);

  let theme = Mui_Theme.useTheme();

  <>
    <MaterialUi_FormControl className={Styles.searchContainer(theme)}>
      <SearchInput
        classes=[
          Input(Styles.searchInput),
          FormControl(Styles.searchFormControl),
        ]
      />
    </MaterialUi_FormControl>
    {switch (searchPodcast, searchType) {
     | (Some(podcast), Episode) =>
       <MaterialUi_Chip
         size=`Medium
         label={str(podcast.title)}
         onDelete={_ => dispatch(ClearEpisodeQueryPodcast)}
       />
     | _ => React.null
     }}
    <ContentTabs
      activeTab=searchType
      onTabChange={contentType => dispatch(SetContentType(contentType))}
    />
  </>;
};
