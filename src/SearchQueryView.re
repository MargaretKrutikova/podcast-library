let str = ReasonReact.string;
let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "SearchQueryStyles"(theme =>
    {
      searchContainer:
        style(
          ~marginBottom=theme |> Utils.spacing(3),
          ~display="flex",
          ~flexDirection="row",
          (),
        ),
      searchFormControl: style(~maxWidth="600px", ()),
      searchInput: style(~fontSize="26px", ()),
    }
  )
];

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let searchQuery = Hooks.useSearchQuery();

  let classes = SearchQueryStyles.useStyles();

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
