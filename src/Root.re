open ReactNetlifyIdentityWidget;

let str = ReasonReact.string;

let getShowIdentityModal = (model: AppCore.model) => model.showIdentityModal;

[@react.component]
let make = () => {
  let showDialog = AppCore.useSelector(getShowIdentityModal);
  let dispatch = AppCore.useDispatch();

  let route = Routing.useAppUrl();
  let identity = useIdentityContext();
  let id = UserIdentity.getUserId(identity);

  let pageToShow =
    React.useMemo2(
      () =>
        switch (route, id) {
        | (MyLibrary, Some(userId)) => <MyLibraryPage userId />
        | (MyEpisodes(id), Some(userId)) =>
          <MyEpisodesPage podcastId=id userId />
        | (Search(urlQuery), _) => <SearchPage userId=id urlQuery />
        | (_, _) => <div />
        },
      (route, id),
    );

  <>
    <MaterialUi_CssBaseline />
    <IdentityModal
      showDialog
      onCloseDialog={() => dispatch(SetShowIdentityModal(false))}
    />
    <AppBar isLoggedIn={UserIdentity.isLoggedIn(identity)} activePage=route />
    <MaterialUi_Container> pageToShow </MaterialUi_Container>
    <RootNotifications />
  </>;
};
