open BsReactstrap;

let str = ReasonReact.string;

module SearchTypeButton = {
  [@react.component]
  let make = (~searchType, ~activeSearchType, ~text) => {
    let dispatch = Hooks.useSearchDispatch();
    let isActive = searchType == activeSearchType;

    <Button
      onClick={_ => dispatch(SetContentType(searchType))}
      outline={!isActive}
      color="info">
      {str(text)}
    </Button>;
  };
};

[@react.component]
let make = () => {
  let dispatch = Hooks.useSearchDispatch();
  let searchQuery = Hooks.useSearchQuery();

  let (searchTerm, setSearchTerm) = React.useState(() => "");
  let handleSearchTermChange = e => {
    let value = ReactEvent.Form.target(e)##value;
    setSearchTerm(_ => value);
  };

  let isActive = searchType => searchQuery.searchType == searchType;

  React.useEffect1(
    () => {
      setSearchTerm(_ => searchQuery.searchTerm);
      None;
    },
    [|searchQuery.searchTerm|],
  );

  <Form
    className="cr-search-form"
    onSubmit={e => ReactEvent.Form.preventDefault(e)}>
    <FormGroup>
      <InputGroup>
        <Input
          _type="search"
          className="cr-search-form__input"
          placeholder="Search..."
          value=searchTerm
          onChange=handleSearchTermChange
        />
        <Button
          color="info"
          onClick={_ => dispatch(EnteredSearchTerm(searchTerm))}
          size="sm">
          <ReactFeather.SearchIcon />
        </Button>
      </InputGroup>
    </FormGroup>
    <Nav tabs=true>
      <NavItem>
        <NavLink
          active={isActive(Episode)}
          onClick={_ => dispatch(SetContentType(Episode))}>
          {str("Episodes")}
        </NavLink>
      </NavItem>
      <NavItem>
        <NavLink
          active={isActive(Podcast)}
          onClick={_ => dispatch(SetContentType(Podcast))}>
          {str("Podcasts")}
        </NavLink>
      </NavItem>
    </Nav>
  </Form>;
};
