module SearchEpisodesQuery =
  ReasonApolloHooks.Query.Make(EpisodeSearch.SearchEpisodes);

let getSearchModel = (model: AppCore.model) => model.search;

[@react.component]
let make = (~savedIds) => {
  let searchModel = AppCore.useSelector(getSearchModel);
  // let shouldSkipSearch =
  // String.trim(searchModel.baseQuery.searchTerm) === "";

  let getEpisodeSearchVariables = (~nextOffset=0, ()): Js.Json.t =>
    EpisodeSearch.makeSearchQuery(
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
      ~updateQuery=EpisodeSearch.fetchMoreUpdateQuery,
      (),
    )
    |> ignore;
  };

  <SearchResultView
    result=full
    renderData={data =>
      <>
        {data##searchEpisodes.results
         ->Belt.Array.map(episode =>
             <EpisodeView
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
