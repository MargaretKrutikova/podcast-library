module GetSavedIdsQuery =
  ReasonApolloHooks.Query.Make(MyLibrary.GetMyLibrarySavedIds);

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
        ~variables=MyLibrary.makeGetSavedIdsQuery(~userId)##variables,
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
let make = (~userId) => {
  let searchType = AppCore.useSelector(getSearchType);

  <>
    <h1> {ReasonReact.string("Search library")} </h1>
    <SearchQueryView />
    {switch (userId) {
     | Some(userId) => <LoggedInSearchView userId searchType />
     | None => <SearchResultsView searchType />
     }}
  </>;
};
