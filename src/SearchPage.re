open BsReactstrap;

let str = ReasonReact.string;

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let searchResult = AppCore.useSelector(model => model.episodeSearchResult);

  React.useEffect0(() => {
    dispatch(FetchPartialLibrary);
    dispatch(RequestedSearch);
    None;
  });

  <Container>
    <h1> {str("Search library")} </h1>
    <SearchQueryView />
    <div>
      {switch (searchResult) {
       | NotAsked => <span> {str("Nothing yet")} </span>
       | Loading(_) => <span> {str("Loading")} </span>
       | Success(res) =>
         res.results
         ->Belt.Array.map(episode =>
             <EpisodeView
               title={episode.title}
               description={episode.description}
               key={episode.listennotesId}
               podcastTitle={episode.podcastTitle}
               pubDate={episode.pubDate}
               listennotesId={episode.listennotesId}
             />
           )
         |> ReasonReact.array
       | Error => <div> {str("Error")} </div>
       }}
    </div>
  </Container>;
};
