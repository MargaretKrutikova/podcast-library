open BsReactstrap;
let str = ReasonReact.string;

module GetMyLibraryQuery =
  ReasonApolloHooks.Query.Make(LibraryQueries.GetMyLibrary);

module GetAllEpisodesQuery =
  ReasonApolloHooks.Query.Make(LibraryQueries.GetAllEpisodes);

[@react.component]
let make = (~userId) => {
  let (activeType, setActiveType) = React.useState(() => ContentType.Episode);
  let isActive = type_ => activeType == type_;

  let getMyLibrary = LibraryQueries.GetMyLibrary.make(~userId, ());

  let (libraryResponse, _full) =
    GetMyLibraryQuery.use(
      ~variables=getMyLibrary##variables,
      ~fetchPolicy=NetworkOnly,
      (),
    );

  let getAllEpisodes = LibraryQueries.GetAllEpisodes.make(~userId, ());
  let (episodesResponse, _full) =
    GetAllEpisodesQuery.use(
      ~variables=getMyLibrary##variables,
      ~fetchPolicy=NetworkOnly,
      (),
    );

  <>
    <h1> {React.string("My library")} </h1>
    <Nav tabs=true>
      <NavItem>
        <NavLink
          active={isActive(Episode)}
          onClick={_ => setActiveType(_ => Episode)}>
          {str("Episodes")}
        </NavLink>
      </NavItem>
      <NavItem>
        <NavLink
          active={isActive(Podcast)}
          onClick={_ => setActiveType(_ => Podcast)}>
          {str("Podcasts")}
        </NavLink>
      </NavItem>
    </Nav>
    {switch (activeType) {
     | Episode =>
       switch (episodesResponse) {
       | NoData => React.null
       | Loading => <div> {React.string("Loading")} </div>
       | Data(response) =>
         response##my_episodes
         ->Belt.Array.map(data =>
             <MyEpisodeView
               key={data##episode##listennotesId}
               episode={LibraryTypes.toMyEpisode(data)}
               podcastItunesId={Some(data##episode##podcast##itunesId)}
               userId
             />
           )
         |> ReasonReact.array
       | Error(_) => <div> {React.string("Error")} </div>
       }
     | Podcast =>
       switch (libraryResponse) {
       | NoData => React.null
       | Loading => <div> {React.string("Loading")} </div>
       | Data(response) =>
         <div>
           {LibraryTypes.toMyLibrary(response).myPodcasts
            ->Belt.Array.map(podcast =>
                <MyPodcastView key={podcast.listennotesId} podcast userId />
              )
            |> React.array}
         </div>
       | Error(_) => <div> {React.string("Error")} </div>
       }
     }}
  </>;
};
