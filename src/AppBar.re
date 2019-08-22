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

[@react.component]
let make = (~isLoggedIn) => {
  let dispatch = AppCore.useDispatch();
  let classes = RootStyles.useStyles();

  <MaterialUi_AppBar color=`Default position=`Static>
    <MaterialUi_Container>
      <MaterialUi_Toolbar disableGutters=true>
        <RouterLink
          variant="h4" href="/" color=`Inherit className={classes.appName}>
          {React.string("Podcast library")}
          <ReactFeather.MicIcon className={classes.appIcon} />
        </RouterLink>
        <div className={classes.desktopSearchBar}> <AppBarSearch /> </div>
        <div className={classes.icons}>
          <MaterialUi_IconButton
            color=`Inherit
            type_=`Button
            size=`Small
            className={classes.hideDesktop}>
            <ReactFeather.SearchIcon />
          </MaterialUi_IconButton>
          {isLoggedIn
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
                 {React.string("Log in")}
               </MaterialUi_Button>}
        </div>
      </MaterialUi_Toolbar>
    </MaterialUi_Container>
  </MaterialUi_AppBar>;
};
