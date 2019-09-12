module SearchPodcastsQuery =
  ReasonApolloHooks.Query.Make(SearchGraphql.SearchPodcasts);

[@react.component]
let make = (~savedIds) => {
  let searchModel = AppCore.useSelector(Selectors.getSearchModel);

  let getPodcastSearchVariables = (~nextOffset=0, ()): Js.Json.t =>
    SearchGraphql.makeSearchPodcastsQuery(searchModel.baseQuery, nextOffset)##variables;

  let (_simple, full) =
    SearchPodcastsQuery.use(
      ~variables=getPodcastSearchVariables(),
      ~notifyOnNetworkStatusChange=true,
      (),
    );

  let handleLoadMore = (~nextOffset) => {
    full.fetchMore(
      ~variables=?Some(getPodcastSearchVariables(~nextOffset, ())),
      ~updateQuery=SearchGraphql.fetchMorePodcasts,
      (),
    )
    |> ignore;
  };

  <SearchResultContainer
    result=full
    renderData={data =>
      <>
        <LibraryGrid>
          {data##searchPodcasts.results
           ->Belt.Array.map(podcast =>
               <PodcastCard
                 key={podcast.listennotesId}
                 podcast
                 isSaved={
                   savedIds->Belt.Array.some(idObj =>
                     idObj##podcastId === podcast.listennotesId
                   )
                 }
               />
             )
           |> ReasonReact.array}
        </LibraryGrid>
        <SearchResultContainer.LoadMoreButton
          nextOffset={data##searchPodcasts.nextOffset}
          total={data##searchPodcasts.total}
          onClick={_ =>
            handleLoadMore(~nextOffset=data##searchPodcasts.nextOffset)
          }
          isFetching={full.networkStatus === ReasonApolloHooks.Types.FetchMore}
        />
      </>
    }
  />;
};
