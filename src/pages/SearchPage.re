module GetSavedIdsQuery =
  ReasonApolloHooks.Query.Make(LibraryQueries.GetMyLibrarySavedIds);

module LoggedInSearchView = {
  [@react.component]
  let make = (~userId) => {
    let (_simple, full) =
      GetSavedIdsQuery.use(
        ~variables=
          LibraryQueries.GetMyLibrarySavedIds.make(~userId, ())##variables,
        (),
      );

    switch (full) {
    | {data: Some(data)} =>
      let savedEpisodeIds = data##my_episodes;
      let savedPodcastIds = data##my_podcasts;

      <SearchResult savedEpisodeIds savedPodcastIds />;

    | _ => <SearchResult />
    };
  };
};

[@react.component]
let make =
  React.memo((~userId) => {
    let hasSearchTerm = AppCore.useSelector(Selectors.hasSearchTerm);

    <PageContainer>
      <SearchForm />
      {hasSearchTerm
         ? switch (userId) {
           | Some(userId) => <LoggedInSearchView userId />
           | None => <SearchResult />
           }
         : React.null}
    </PageContainer>;
  });
