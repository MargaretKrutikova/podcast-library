open BsReactstrap;
open ReactNetlifyIdentityWidget;

let str = ReasonReact.string;

[@react.component]
let make = () => {
  let (showDialog, setShowDialog) = React.useState(() => false);

  let url = ReasonReactRouter.useUrl();
  let identity = useIdentityContext();

  let id = UserIdentity.getUserId(identity);
  let pageToShow =
    switch (url.path, id) {
    | (["my-library"], Some(userId)) => <MyLibraryPage userId />
    | (["my-library", id, "episodes"], Some(userId)) =>
      <MyEpisodesPage podcastId=id userId />
    | (_, _) => <SearchPage />
    };

  <div>
    <IdentityModal
      showDialog
      onCloseDialog={() => setShowDialog(_ => false)}
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
             : <NavLink href="#" onClick={_ => setShowDialog(_ => true)}>
                 {str("Log in")}
               </NavLink>}
        </NavItem>
      </Nav>
    </Navbar>
    <Container> pageToShow </Container>
    <RootNotifications />
  </div>;
};
