open BsReactstrap;

let str = ReasonReact.string;

module LoadMoreButton = {
  [@react.component]
  let make = (~nextOffset, ~total, ~isFetching) => {
    let dispatch = Hooks.useSearchDispatch();
    let hasMore = nextOffset < total;

    hasMore
      ? <div className="load-more-button-container">
          <Button
            color="info"
            onClick={_ => dispatch(RequestMoreResults)}
            disabled=isFetching>
            {str("Fetch more")}
          </Button>
        </div>
      : ReasonReact.null;
  };
};

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let searchDispatch = Hooks.useSearchDispatch();
  let searchResult = Hooks.useSearchResult();

  React.useEffect0(() => {
    dispatch(FetchLibraryIds);
    searchDispatch(RequestedSearch);
    None;
  });

  <>
    <h1> {str("Search library")} </h1>
    <SearchQueryView />
    <div>
      {switch (searchResult) {
       | PodcastResult(NotAsked)
       | EpisodeResult(NotAsked) => <span> {str("Nothing yet")} </span>
       | PodcastResult(Loading(_, _))
       | EpisodeResult(Loading(_, _)) => <span> {str("Loading")} </span>
       | PodcastResult(Error)
       | EpisodeResult(Error) => <div> {str("Error")} </div>
       | PodcastResult(Success(data, _) as result)
       | PodcastResult(FetchingMore(data, _) as result) =>
         <>
           {data.results
            ->Belt.Array.map(podcast =>
                <PodcastView key={podcast.listennotesId} podcast />
              )
            |> ReasonReact.array}
           <LoadMoreButton
             nextOffset={data.nextOffset}
             total={data.total}
             isFetching={SearchModel.isFetchingMore(result)}
           />
         </>

       | EpisodeResult(Success(data, _) as result)
       | EpisodeResult(FetchingMore(data, _) as result) =>
         <>
           {data.results
            ->Belt.Array.map(episode =>
                <EpisodeView key={episode.listennotesId} episode />
              )
            |> ReasonReact.array}
           <LoadMoreButton
             nextOffset={data.nextOffset}
             total={data.total}
             isFetching={SearchModel.isFetchingMore(result)}
           />
         </>
       }}
    </div>
  </>;
};
