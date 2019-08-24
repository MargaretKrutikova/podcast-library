let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "RootStyles"(theme =>
    {
      desktopSearchBar:
        style(~display="none", ())
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=
              style(
                ~flex="1 1 auto",
                ~display="flex",
                ~justifyContent="center",
                ~alignItems="center",
                (),
              ),
          ),
      searchInput:
        style(
          ~maxWidth="400px",
          ~fontSize="1.3rem",
          ~position="absolute",
          ~left="50%",
          ~transform="translateX(-50%)",
          (),
        ),
      hideDesktop:
        style()
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=style(~display="none", ()),
          ),
      icons: style(~marginLeft="auto", ()),
      appName:
        style(~fontWeight="100", ~fontSize="1.5rem", ())
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=style(~fontSize="1.7rem", ()),
          ),
      appIcon: style(~marginLeft=theme |> Utils.spacing(1), ()),
    }
  )
];

let isSearchPage = page =>
  switch (page) {
  | Routing.Search(_) => true
  | _ => false
  };

[@react.component]
let make = (~isLoggedIn, ~activePage) => {
  let dispatch = AppCore.useDispatch();
  let classes = RootStyles.useStyles();
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
               <SearchInput className={classes.searchInput} />
             </div>
           : React.null}
        <div className={classes.icons}>
          <RouterLink href={Routing.getUrlFromRoute(Search({query: ""}))}>
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
