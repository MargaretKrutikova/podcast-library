[@react.component]
let make = (~className=?, ~classes: option(MaterialUi_Input.Classes.t)=?) => {
  let dispatch = AppCore.useDispatch();

  let globalSearchTerm = AppCore.useSelector(Selectors.getSearchTerm);
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
  let handleSearch = _ => dispatch(EnteredSearchTerm(searchTerm));

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
    ?className
    ?classes
    endAdornment={
      <MaterialUi_IconButton type_=`Button onClick=handleSearch size=`Small>
        <ReactFeather.SearchIcon />
      </MaterialUi_IconButton>
    }
  />;
};
