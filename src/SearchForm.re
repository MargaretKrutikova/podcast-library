let str = ReasonReact.string;

let getSearchType = (model: AppCore.model) => model.search.searchType;
let getSearchTerm = (model: AppCore.model) =>
  model.search.baseQuery.searchTerm;

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let searchType = AppCore.useSelector(getSearchType);
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
      activeTab=searchType
      onTabChange={contentType => dispatch(SetContentType(contentType))}
    />
  </>;
};
