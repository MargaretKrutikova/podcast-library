let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "SearchStyles"(_ =>
    {searchInput: style(~maxWidth="400px", ~fontSize="1.3rem", ())}
  )
];

let getSearchTerm = (model: AppCore.model) =>
  model.search.baseQuery.searchTerm;

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();

  let classes = SearchStyles.useStyles();
  let globalSearchTerm = AppCore.useSelector(getSearchTerm);
  let (searchTerm, setSearchTerm) = React.useState(() => "");

  React.useEffect1(
    () => {
      setSearchTerm(_ => globalSearchTerm);
      None;
    },
    [|globalSearchTerm|],
  );

  let handleSearchChange = e => {
    let value = Utils.getInputValue(e);
    setSearchTerm(_ => value);
  };
  let handleSearch = _ => {
    dispatch(EnteredSearchTerm(searchTerm));
    Routing.pushRoute(Search({query: searchTerm}));
  };

  let handleKeyDown = e => {
    switch (Utils.onKeyDown(e)) {
    | Enter => handleSearch()
    | _ => ()
    };
  };

  <MaterialUi_Input
    type_="search"
    placeholder="Search..."
    value=searchTerm
    onChange=handleSearchChange
    fullWidth=true
    onKeyDown=handleKeyDown
    className={classes.searchInput}
    endAdornment={
      <MaterialUi_IconButton onClick=handleSearch>
        <ReactFeather.SearchIcon />
      </MaterialUi_IconButton>
    }
  />;
};
