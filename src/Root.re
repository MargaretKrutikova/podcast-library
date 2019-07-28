open BsReactstrap;

let str = ReasonReact.string;

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

  let pageToShow =
    switch (url.path) {
    | ["my-library"] => <MyLibraryPage />
    | ["my-library", id, "episodes"] => <MyEpisodesPage podcastId=id />
    | _ => <SearchPage />
    };

  <div>
    <Navbar>
      <NavbarBrand href="/"> {str("Podcast library")} </NavbarBrand>
      <Nav className="ml-auto">
        <NavItem>
          <RouterLink className="nav-link" href="/search">
            {str("Search")}
          </RouterLink>
        </NavItem>
        <NavItem>
          <RouterLink className="nav-link" href="/my-library">
            {str("My Library")}
          </RouterLink>
        </NavItem>
      </Nav>
    </Navbar>
    <Container> pageToShow </Container>
    <RootNotifications />
  </div>;
};
