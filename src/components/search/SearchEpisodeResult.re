module SearchEpisodesQuery =
  ReasonApolloHooks.Query.Make(SearchGraphql.SearchEpisodes);

[@react.component]
let make = (~savedIds) => {
  let searchModel = AppCore.useSelector(Selectors.getSearchModel);

  let getEpisodeSearchVariables = (~nextOffset=0, ()): Js.Json.t =>
    SearchGraphql.makeSearchEpisodesQuery(searchModel, nextOffset)##variables;

  let (_simple, full) =
    SearchEpisodesQuery.use(
      ~variables=getEpisodeSearchVariables(),
      ~notifyOnNetworkStatusChange=true,
      (),
    );

  let handleLoadMore = (~nextOffset) => {
    full.fetchMore(
      ~variables=?Some(getEpisodeSearchVariables(~nextOffset, ())),
      ~updateQuery=SearchGraphql.fetchMoreEpisodes,
      (),
    )
    |> ignore;
  };

  <SearchResultContainer
    result=full
    renderData={data =>
      <>
        <LibraryGrid>
          {data##searchEpisodes.results
           ->Belt.Array.map(episode =>
               <EpisodeCard
                 key={episode.listennotesId}
                 episode
                 isSaved={
                   savedIds->Belt.Array.some(idObj =>
                     idObj##episodeId === episode.listennotesId
                   )
                 }
               />
             )
           |> ReasonReact.array}
        </LibraryGrid>
        <SearchResultContainer.LoadMoreButton
          nextOffset={data##searchEpisodes.nextOffset}
          total={data##searchEpisodes.total}
          onClick={_ =>
            handleLoadMore(~nextOffset=data##searchEpisodes.nextOffset)
          }
          isFetching={full.networkStatus === ReasonApolloHooks.Types.FetchMore}
        />
      </>
    }
  />;
};
