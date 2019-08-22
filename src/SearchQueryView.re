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
      searchInput: style(~fontSize="24px", ()),
    }
  )
];

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let searchQuery = Hooks.useSearchQuery();

  let (searchTerm, setSearchTerm) = React.useState(() => "");
  let handleSearchTermChange = e => {
    let value = Utils.getInputValue(e);
    setSearchTerm(_ => value);
  };

  let handleKeyDown = e => {
    switch (Utils.onKeyDown(e)) {
    | Enter => dispatch(EnteredSearchTerm(searchTerm))
    | _ => ()
    };
  };

  React.useEffect1(
    () => {
      setSearchTerm(_ => searchQuery.searchTerm);
      None;
    },
    [|searchQuery.searchTerm|],
  );

  let classes = SearchQueryStyles.useStyles();

  <>
    <form
      onSubmit={e => {
        ReactEvent.Form.preventDefault(e);
        dispatch(EnteredSearchTerm(searchTerm));
      }}>
      <MaterialUi_FormControl className={classes.searchContainer}>
        <MaterialUi_Input
          type_="search"
          placeholder="Search..."
          value=searchTerm
          onChange=handleSearchTermChange
          fullWidth=true
          onKeyDown=handleKeyDown
          classes=[Input(classes.searchInput)]
          endAdornment={
            <MaterialUi_IconButton
              color=`Primary
              type_=`Button
              onClick={_ => dispatch(EnteredSearchTerm(searchTerm))}
              size=`Small>
              <ReactFeather.SearchIcon />
            </MaterialUi_IconButton>
          }
        />
      </MaterialUi_FormControl>
    </form>
    <ContentTabs
      activeTab={searchQuery.searchType}
      onTabChange={contentType => dispatch(SetContentType(contentType))}
    />
  </>;
};
