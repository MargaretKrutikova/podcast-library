open BsReactstrap;
open ReactNetlifyIdentityWidget;

let str = ReasonReact.string;

let getShowIdentityModal = (model: AppCore.model) => model.showIdentityModal;

[@react.component]
let make = () => {
  let showDialog = AppCore.useSelector(getShowIdentityModal);
  let dispatch = AppCore.useDispatch();

  let url = ReasonReactRouter.useUrl();
  let identity = useIdentityContext();

  let id = UserIdentity.getUserId(identity);
  let pageToShow =
    switch (url.path, id) {
    | (["my-library"], Some(userId)) => <MyLibraryPage userId />
    | (["my-library", id, "episodes"], Some(userId)) =>
      <MyEpisodesPage podcastId=id userId />
    | (_, _) => <SearchPage userId=id />
    };

  <div>
    <IdentityModal
      showDialog
      onCloseDialog={() => dispatch(SetShowIdentityModal(false))}
    />
    <Navbar>
      <NavbarBrand href="/"> {str("Podcast library")} </NavbarBrand>
      <Nav className="ml-auto">
        <NavItem>
          <RouterLink className="nav-link" href="/search">
            {str("Search")}
          </RouterLink>
        </NavItem>
        <NavItem>
          {UserIdentity.isLoggedIn(identity)
             ? <RouterLink className="nav-link" href="/my-library">
                 {str("My Library")}
               </RouterLink>
             : <NavLink
                 href="#"
                 onClick={() => dispatch(SetShowIdentityModal(true))}>
                 {str("Log in")}
               </NavLink>}
        </NavItem>
      </Nav>
    </Navbar>
    <Container> pageToShow </Container>
    <RootNotifications />
  </div>;
};
