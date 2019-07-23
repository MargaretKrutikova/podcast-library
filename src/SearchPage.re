open BsReactstrap;

let str = ReasonReact.string;

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

  <Container>
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
       | PodcastResult(Success(data, _)) =>
         data.results
         ->Belt.Array.map(podcast =>
             <PodcastView key={podcast.listennotesId} podcast />
           )
         |> ReasonReact.array

       | EpisodeResult(Success(data, _)) =>
         data.results
         ->Belt.Array.map(episode =>
             <EpisodeView key={episode.listennotesId} episode />
           )
         |> ReasonReact.array
       }}
    </div>
  </Container>;
};
