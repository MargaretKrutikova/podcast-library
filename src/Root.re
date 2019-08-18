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
      <RouterLink href="/">
        <span className="navbar-brand"> {str("Podcast library")} </span>
      </RouterLink>
      <Nav className="ml-auto">
        <NavItem>
          <RouterLink className="nav-link px-2" href="/search">
            <ReactFeather.SearchIcon />
          </RouterLink>
        </NavItem>
        {UserIdentity.isLoggedIn(identity)
           ? <>
               <NavItem>
                 <NavLink
                   className="px-2"
                   href="#"
                   onClick={() => dispatch(SetShowIdentityModal(true))}>
                   <ReactFeather.UserIcon />
                 </NavLink>
               </NavItem>
               <NavItem>
                 <RouterLink className="nav-link px-2" href="/my-library">
                   {str("My Library")}
                 </RouterLink>
               </NavItem>
             </>
           : <NavItem>
               <NavLink
                 href="#"
                 onClick={() => dispatch(SetShowIdentityModal(true))}>
                 {str("Log in")}
               </NavLink>
             </NavItem>}
      </Nav>
    </Navbar>
    <Container> pageToShow </Container>
    <RootNotifications />
  </div>;
};
