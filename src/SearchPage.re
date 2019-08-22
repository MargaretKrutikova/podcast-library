module GetSavedIdsQuery =
  ReasonApolloHooks.Query.Make(LibraryQueries.GetMyLibrarySavedIds);

module SearchResultsView = {
  [@react.component]
  let make =
      (
        ~searchType: ContentType.t,
        ~savedPodcastsIds=[||],
        ~savedEpisodesIds=[||],
      ) => {
    switch (searchType) {
    | Episode => <SearchEpisodesView savedIds=savedEpisodesIds />
    | Podcast => <SearchPodcastsView savedIds=savedPodcastsIds />
    };
  };
};

module LoggedInSearchView = {
  [@react.component]
  let make = (~userId, ~searchType: ContentType.t) => {
    let (_simple, full) =
      GetSavedIdsQuery.use(
        ~variables=
          LibraryQueries.GetMyLibrarySavedIds.make(~userId, ())##variables,
        (),
      );

    switch (full) {
    | {data: Some(data)} =>
      let savedEpisodesIds = data##my_episodes;
      let savedPodcastsIds = data##my_podcasts;

      <SearchResultsView searchType savedEpisodesIds savedPodcastsIds />;

    | _ => <SearchResultsView searchType />
    };
  };
};

let getSearchType = (model: AppCore.model) => model.search.searchType;

[@react.component]
let make =
  React.memo((~userId, ~urlQuery: Routing.searchQuery) => {
    let searchType = AppCore.useSelector(getSearchType);
    let dispatch = AppCore.useDispatch();

    React.useEffect1(
      () => {
        dispatch(SetSearchModelFromQuery(urlQuery));
        None;
      },
      [|urlQuery|],
    );

    <>
      <PageTitle title="Search results" />
      <SearchQueryView />
      {switch (userId) {
       | Some(userId) => <LoggedInSearchView userId searchType />
       | None => <SearchResultsView searchType />
       }}
    </>;
  });
