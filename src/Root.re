open ReactNetlifyIdentityWidget;

let str = ReasonReact.string;
let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "RootStyles"(theme =>
    {
      grow:
        style(~display="none", ())
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=
              style(
                ~flex="1 1 auto",
                ~display="flex",
                ~justifyContent="center",
                (),
              ),
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
      searchInput: style(~maxWidth="400px", ~fontSize="1.5rem", ()),
      appIcon: style(~marginLeft=theme |> Utils.spacing(1), ()),
      appBar:
        style(
          ~backgroundColor=
            Utils.getPrimaryColor(theme)
            |> MaterialUi.Theme.PaletteColor.darkGet,
          (),
        ),
    }
  )
];

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
  let classes = RootStyles.useStyles();

  <div>
    <MaterialUi_CssBaseline />
    <IdentityModal
      showDialog
      onCloseDialog={() => dispatch(SetShowIdentityModal(false))}
    />
    <MaterialUi_AppBar color=`Default position=`Static>
      <MaterialUi_Container>
        <MaterialUi_Toolbar disableGutters=true>
          <RouterLink
            variant="h4" href="/" color=`Inherit className={classes.appName}>
            {str("Podcast library")}
            <ReactFeather.MicIcon className={classes.appIcon} />
          </RouterLink>
          <div className={classes.grow}>
            <MaterialUi_Input
              type_="search"
              placeholder="Search..."
              fullWidth=true
              className={classes.searchInput}
              endAdornment={
                <RouterLink href="/search" underline=`None>
                  <ReactFeather.SearchIcon />
                </RouterLink>
              }
            />
          </div>
          <div className={classes.icons}>
            <MaterialUi_IconButton
              color=`Inherit
              type_=`Button
              size=`Small
              className={classes.hideDesktop}>
              <ReactFeather.SearchIcon />
            </MaterialUi_IconButton>
            {UserIdentity.isLoggedIn(identity)
               ? <>
                   <MaterialUi_IconButton
                     color=`Inherit
                     onClick={_ => dispatch(SetShowIdentityModal(true))}>
                     <ReactFeather.UserIcon />
                   </MaterialUi_IconButton>
                   <RouterLink href="/my-library" color=`Inherit>
                     <ReactFeather.BookmarkIcon />
                   </RouterLink>
                 </>
               : <MaterialUi_Button
                   color=`Inherit
                   onClick={_ => dispatch(SetShowIdentityModal(true))}>
                   {str("Log in")}
                 </MaterialUi_Button>}
          </div>
        </MaterialUi_Toolbar>
      </MaterialUi_Container>
    </MaterialUi_AppBar>
    <MaterialUi_Container> pageToShow </MaterialUi_Container>
    <RootNotifications />
  </div>;
};
