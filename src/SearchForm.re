let str = ReasonReact.string;

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let searchQuery = Hooks.useSearchQuery();

  let classes = AppStyles.AppStyles.useStyles();

  <>
    <MaterialUi_FormControl className={classes.searchContainer}>
      <SearchInput
        classes=[
          Input(classes.searchInput),
          FormControl(classes.searchFormControl),
        ]
      />
    </MaterialUi_FormControl>
    <ContentTabs
      activeTab={searchQuery.searchType}
      onTabChange={contentType => dispatch(SetContentType(contentType))}
    />
  </>;
};
