let isSearchPage = page =>
  switch (page) {
  | Routing.Search(_) => true
  | _ => false
  };

[@react.component]
let make = (~isLoggedIn, ~activePage) => {
  let dispatch = AppCore.useDispatch();
  let classes = AppStyles.AppStyles.useStyles();
  let (userMenuAnchor, setUserMenuAnchor) = React.useState(() => None);

  <MaterialUi_AppBar color=`Default position=`Static>
    <MaterialUi_Container>
      <MaterialUi_Toolbar disableGutters=true>
        <RouterLink
          variant="h4" href="/" color=`Inherit className={classes.appName}>
          {React.string("Podcast library")}
          <ReactFeather.MicIcon className={classes.appIcon} />
        </RouterLink>
        {!isSearchPage(activePage)
           ? <div className={classes.desktopSearchBar}>
               <SearchInput className={classes.appBarSearchInput} />
             </div>
           : React.null}
        <div className={classes.appBarIcons}>
          <RouterLink href={Routing.getUrlFromRoute(SearchEmpty)}>
            <MaterialUi_IconButton component={`String("span")}>
              <ReactFeather.SearchIcon />
            </MaterialUi_IconButton>
          </RouterLink>
          {isLoggedIn
             ? <>
                 <MaterialUi_IconButton
                   onClick={e => {
                     let anchor = e->ReactEvent.Mouse.currentTarget;
                     setUserMenuAnchor(_ => Some(anchor));
                   }}>
                   <ReactFeather.UserIcon />
                 </MaterialUi_IconButton>
                 <UserMenu
                   anchorEl=userMenuAnchor
                   onLogout={() => Routing.pushRoute(SearchEmpty)}
                   onClose={(_, _) => setUserMenuAnchor(_ => None)}
                 />
                 <RouterLink href={Routing.getUrlFromRoute(MyLibrary)}>
                   <MaterialUi_IconButton component={`String("span")}>
                     <ReactFeather.BookmarkIcon />
                   </MaterialUi_IconButton>
                 </RouterLink>
               </>
             : <MaterialUi_Button
                 onClick={_ => dispatch(SetShowIdentityModal(true))}>
                 {React.string("Log in")}
               </MaterialUi_Button>}
        </div>
      </MaterialUi_Toolbar>
    </MaterialUi_Container>
  </MaterialUi_AppBar>;
};
