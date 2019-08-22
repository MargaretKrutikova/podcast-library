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

  let searchUrl = Routing.getUrlFromRoute(Search({query: searchTerm}));

  <MaterialUi_Input
    type_="search"
    placeholder="Search..."
    value=searchTerm
    onChange=handleSearchChange
    fullWidth=true
    className={classes.searchInput}
    endAdornment={
      <RouterLink href=searchUrl underline=`None>
        <ReactFeather.SearchIcon />
      </RouterLink>
    }
  />;
};
