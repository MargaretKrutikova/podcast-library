module SearchPodcastsQuery =
  ReasonApolloHooks.Query.Make(SearchGraphql.SearchPodcasts);

let getSearchModel = (model: AppCore.model) => model.search;

[@react.component]
let make = (~savedIds) => {
  let searchModel = AppCore.useSelector(getSearchModel);

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

  <SearchResultView
    result=full
    renderData={data =>
      <>
        <LibraryGrid.Container>
          {data##searchPodcasts.results
           ->Belt.Array.map(podcast =>
               <LibraryGrid.Item key={podcast.listennotesId}>
                 <PodcastView
                   podcast
                   isSaved={
                     savedIds->Belt.Array.some(idObj =>
                       idObj##podcastId === podcast.listennotesId
                     )
                   }
                 />
               </LibraryGrid.Item>
             )
           |> ReasonReact.array}
        </LibraryGrid.Container>
        <SearchResultView.LoadMoreButton
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
