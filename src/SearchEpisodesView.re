module SearchEpisodesQuery =
  ReasonApolloHooks.Query.Make(SearchGraphql.SearchEpisodes);

let getSearchModel = (model: AppCore.model) => model.search;

[@react.component]
let make = (~savedIds) => {
  let searchModel = AppCore.useSelector(getSearchModel);

  let getEpisodeSearchVariables = (~nextOffset=0, ()): Js.Json.t =>
    SearchGraphql.makeSearchEpisodesQuery(
      searchModel.baseQuery,
      nextOffset,
      searchModel.episodeQuery,
    )##variables;

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

  <SearchResultView
    result=full
    renderData={data =>
      <>
        <LibraryGrid.Container>
          {data##searchEpisodes.results
           ->Belt.Array.map(episode =>
               <LibraryGrid.Item key={episode.listennotesId}>
                 <EpisodeView
                   episode
                   isSaved={
                     savedIds->Belt.Array.some(idObj =>
                       idObj##episodeId === episode.listennotesId
                     )
                   }
                 />
               </LibraryGrid.Item>
             )
           |> ReasonReact.array}
        </LibraryGrid.Container>
        <SearchResultView.LoadMoreButton
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
