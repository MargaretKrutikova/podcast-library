let str = ReasonReact.string;

module GetAllPodcastsQuery =
  ReasonApolloHooks.Query.Make(LibraryQueries.GetAllPodcasts);

module GetAllEpisodesQuery =
  ReasonApolloHooks.Query.Make(LibraryQueries.GetAllEpisodes);

[@react.component]
let make = (~userId) => {
  let (activeType, setActiveType) = React.useState(() => ContentType.Episode);

  let getAllPodcats = LibraryQueries.GetAllPodcasts.make(~userId, ());

  let (podcastsResponse, _full) =
    GetAllPodcastsQuery.use(
      ~variables=getAllPodcats##variables,
      ~fetchPolicy=NetworkOnly,
      (),
    );

  let getAllEpisodes = LibraryQueries.GetAllEpisodes.make(~userId, ());
  let (episodesResponse, _full) =
    GetAllEpisodesQuery.use(
      ~variables=getAllEpisodes##variables,
      ~fetchPolicy=NetworkOnly,
      (),
    );

  <>
    <PageTitle title="My library" />
    <ContentTabs
      activeTab=activeType
      onTabChange={contentType => setActiveType(_ => contentType)}
    />
    {switch (activeType) {
     | Episode =>
       switch (episodesResponse) {
       | NoData => React.null
       | Loading => <div> {React.string("Loading")} </div>
       | Data(response) =>
         <LibraryGrid.Container>
           {response##my_episodes
            ->Belt.Array.map(data =>
                <LibraryGrid.Item key={data##episode##listennotesId}>
                  <MyEpisodeView
                    episode={LibraryTypes.toMyEpisode(data)}
                    podcastItunesId={Some(data##episode##podcast##itunesId)}
                    userId
                  />
                </LibraryGrid.Item>
              )
            |> ReasonReact.array}
         </LibraryGrid.Container>

       | Error(_) => <div> {React.string("Error")} </div>
       }
     | Podcast =>
       switch (podcastsResponse) {
       | NoData => React.null
       | Loading => <div> {React.string("Loading")} </div>
       | Data(response) =>
         <LibraryGrid.Container>
           {response##my_podcasts
            ->Belt.Array.map(data =>
                <LibraryGrid.Item key={data##podcastId}>
                  <MyPodcastView
                    id={data##podcastId}
                    description={data##podcast##description}
                    title={data##podcast##title}
                    publisher={data##podcast##publisher}
                    itunesId={data##podcast##itunesId}
                    userId
                  />
                </LibraryGrid.Item>
              )
            |> React.array}
         </LibraryGrid.Container>
       | Error(_) => <div> {React.string("Error")} </div>
       }
     }}
  </>;
};
