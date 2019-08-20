open ReactNetlifyIdentityWidget;

let str = ReasonReact.string;
let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "RootStyles"({
    grow: style(~flex="1 1 auto", ()),
    searchInput: style(~fontSize="24px", ()),
  })
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
    <IdentityModal
      showDialog
      onCloseDialog={() => dispatch(SetShowIdentityModal(false))}
    />
    // "#A3CEF1"
    <MaterialUi_AppBar color=`Primary position=`Static>
      <MaterialUi_Container>
        <MaterialUi_Toolbar disableGutters=true>
          <RouterLink variant="h4" href="/" underline=`None>
            {str("Podcast library")}
          </RouterLink>
          <div className={classes.grow} />
          <RouterLink href="/search" underline=`None>
            <MaterialUi_IconButton color=`Inherit>
              <ReactFeather.SearchIcon />
            </MaterialUi_IconButton>
          </RouterLink>
          {UserIdentity.isLoggedIn(identity)
             ? <>
                 <MaterialUi_IconButton
                   color=`Inherit
                   onClick={_ => dispatch(SetShowIdentityModal(true))}>
                   <ReactFeather.UserIcon />
                 </MaterialUi_IconButton>
                 <RouterLink href="/my-library">
                   {str("My Library")}
                 </RouterLink>
               </>
             : <MaterialUi_Button
                 color=`Inherit
                 onClick={_ => dispatch(SetShowIdentityModal(true))}>
                 {str("Log in")}
               </MaterialUi_Button>}
        </MaterialUi_Toolbar>
      </MaterialUi_Container>
    </MaterialUi_AppBar>
    <MaterialUi_Container> pageToShow </MaterialUi_Container>
    <RootNotifications />
  </div>;
};
