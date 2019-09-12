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
    | Episode => <SearchEpisodeResult savedIds=savedEpisodesIds />
    | Podcast => <SearchPodcastResult savedIds=savedPodcastsIds />
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

[@react.component]
let make =
  React.memo((~userId, ~urlQuery: SearchQs.t) => {
    let searchType = AppCore.useSelector(Selectors.getSearchType);
    let hasSearchTerm = AppCore.useSelector(Selectors.hasSearchTerm);

    let dispatch = AppCore.useDispatch();

    React.useEffect1(
      () => {
        dispatch(SetSearchModelFromQuery(urlQuery));
        None;
      },
      [|urlQuery|],
    );

    <PageContainer>
      <SearchForm />
      {hasSearchTerm
         ? switch (userId) {
           | Some(userId) => <LoggedInSearchView userId searchType />
           | None => <SearchResultsView searchType />
           }
         : React.null}
    </PageContainer>;
  });
