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

  <PageContainer>
    <PageTitle title="My library" />
    <ContentTabs
      activeTab=activeType
      onTabChange={contentType => setActiveType(_ => contentType)}
    />
    {switch (activeType) {
     | Episode =>
       switch (episodesResponse) {
       | NoData => React.null
       | Loading => <Spinner />
       | Data(response) =>
         <LibraryGrid>
           {response##my_episodes
            ->Belt.Array.map(data =>
                <MyEpisodeCard
                  key={data##episode##id}
                  episode={LibraryTypes.toMyEpisode(data)}
                  podcastItunesId={Some(data##episode##podcast##itunesId)}
                  userId
                />
              )
            |> ReasonReact.array}
         </LibraryGrid>

       | Error(_) => <div> {React.string("Error")} </div>
       }
     | Podcast =>
       switch (podcastsResponse) {
       | NoData => React.null
       | Loading => <Spinner />
       | Data(response) =>
         <LibraryGrid>
           {response##my_podcasts
            ->Belt.Array.map(LibraryTypes.toMyPodcast)
            ->Belt.Array.map(podcast =>
                <MyPodcastCard
                  key={podcast.id}
                  id={podcast.id}
                  description={podcast.description}
                  title={podcast.title}
                  publisher={podcast.publisher}
                  itunesId={podcast.itunesId}
                  addedDate={podcast.addedDate}
                  image={podcast.image}
                  userId
                />
              )
            |> React.array}
         </LibraryGrid>
       | Error(_) => <div> {React.string("Error")} </div>
       }
     }}
  </PageContainer>;
};
