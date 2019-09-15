module SearchQuery = ReasonApolloHooks.Query.Make(SearchGraphql.Search);

[@react.component]
let make = (~savedEpisodeIds=[||], ~savedPodcastIds=[||]) => {
  let searchModel = AppCore.useSelector(Selectors.getSearchModel);

  let getSearchVariables = (~nextOffset=0, ()): Js.Json.t =>
    SearchGraphql.makeSearchQuery(searchModel, nextOffset)##variables;

  let (_simple, full) =
    SearchQuery.use(
      ~variables=getSearchVariables(),
      ~notifyOnNetworkStatusChange=true,
      (),
    );

  let handleLoadMore = (~nextOffset) => {
    full.fetchMore(
      ~variables=?Some(getSearchVariables(~nextOffset, ())),
      ~updateQuery=SearchGraphql.fetchMoreUpdateQuery,
      (),
    )
    |> ignore;
  };

  <SearchResultContainer
    result=full
    renderData={data =>
      <>
        <LibraryGrid>
          {data##search.results
           ->Belt.Array.map(result =>
               switch (result) {
               | `Episode(episode) =>
                 <EpisodeCard
                   key={episode.id}
                   episode
                   isSaved={
                     savedEpisodeIds->Belt.Array.some(idObj =>
                       idObj##episodeId === episode.id
                     )
                   }
                 />
               | `Podcast(podcast) =>
                 <PodcastCard
                   key={podcast.id}
                   podcast
                   isSaved={
                     savedPodcastIds->Belt.Array.some(idObj =>
                       idObj##podcastId === podcast.id
                     )
                   }
                 />
               }
             )
           |> ReasonReact.array}
        </LibraryGrid>
        <SearchResultContainer.LoadMoreButton
          nextOffset={data##search.nextOffset}
          total={data##search.total}
          onClick={_ => handleLoadMore(~nextOffset=data##search.nextOffset)}
          isFetching={full.networkStatus === ReasonApolloHooks.Types.FetchMore}
        />
      </>
    }
  />;
};
