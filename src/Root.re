let str = ReasonReact.string;

let getShowIdentityModal = (model: AppCore.model) => model.showIdentityModal;

[@react.component]
let make = () => {
  let showDialog = AppCore.useSelector(getShowIdentityModal);
  let dispatch = AppCore.useDispatch();

  let route = Routing.useAppUrl();
  let identity = UserIdentity.Context.useIdentityContext();

  let id = identity.user->Belt.Option.map(u => u.id);

  let pageToShow =
    React.useMemo2(
      () =>
        switch (route, id) {
        | (MyLibrary, Some(userId)) => <MyLibraryPage userId />
        | (MyEpisodes(id), Some(userId)) =>
          <MyEpisodesPage podcastId=id userId />
        | (Search(_), _) => <SearchPage userId=id />
        | (_, _) => <div />
        },
      (route, id),
    );
  let closeDialog = () => {
    dispatch(SetShowIdentityModal(false));
  };
  let handleLogin = () => {
    closeDialog();
    Routing.pushRoute(MyLibrary);
  };
  <>
    <MaterialUi_CssBaseline />
    <IdentityDialog open_=showDialog onLogin=handleLogin onClose=closeDialog />
    <AppBar isLoggedIn={identity.isLoggedIn} activePage=route />
    <MaterialUi_Container> pageToShow </MaterialUi_Container>
    <RootNotifications />
  </>;
};
