open BsReactstrap;

let str = ReasonReact.string;

module SearchTypeButton = {
  [@react.component]
  let make = (~searchType, ~activeSearchType, ~text) => {
    let dispatch = AppCore.useDispatch();
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
  let dispatch = AppCore.useDispatch();
  let searchQuery = Hooks.useSearchQuery();

  let handleSearchChange = e =>
    dispatch(EnteredSearchTerm(ReactEvent.Form.target(e)##value));
  let isActive = searchType => searchQuery.searchType == searchType;

  <Form
    className="cr-search-form"
    onSubmit={e => ReactEvent.Form.preventDefault(e)}>
    <FormGroup>
      <InputGroup>
        <Input
          _type="search"
          className="cr-search-form__input"
          placeholder="Search..."
          value={searchQuery.searchTerm}
          onChange=handleSearchChange
        />
        <Button onClick={_ => dispatch(RequestedSearch)} size="sm">
          {str("Search")}
        </Button>
      </InputGroup>
    </FormGroup>
    {!searchQuery.hasSearchResult
       ? <ButtonGroup>
           <SearchTypeButton
             searchType=ContentType.Episode
             activeSearchType={searchQuery.searchType}
             text="Episodes"
           />
           <SearchTypeButton
             searchType=ContentType.Podcast
             activeSearchType={searchQuery.searchType}
             text="Podcasts"
           />
         </ButtonGroup>
       : <Nav tabs=true>
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
         </Nav>}
  </Form>;
};
