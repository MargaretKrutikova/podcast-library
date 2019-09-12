module MyEpisodesQuery =
  ReasonApolloHooks.Query.Make(LibraryQueries.GetMyEpisodes);

let str = ReasonReact.string;

[@react.component]
let make = (~podcastId: string, ~userId) => {
  let getMyEpisodes =
    LibraryQueries.GetMyEpisodes.make(~userId, ~podcastId, ());

  let (simple, _full) =
    MyEpisodesQuery.use(
      ~variables=getMyEpisodes##variables,
      ~fetchPolicy=NetworkOnly,
      (),
    );

  <>
    <h1> {str("My episodes")} </h1>
    {switch (simple) {
     | NoData => React.null
     | Loading => <Spinner />
     | Error(_) => <div> {str("Error")} </div>
     | Data(response) =>
       if (response##my_episodes->Belt.Array.length === 0) {
         ReasonReactRouter.replace("/my-library");
       };
       let podcast = response##podcasts->Belt.Array.get(0);
       let podcastItunesId =
         podcast->Belt.Option.mapWithDefault(None, data =>
           Some(data##itunesId)
         );

       <PageContainer>
         <div>
           {switch (podcast) {
            | Some(data) => <h3> {str(data##title)} </h3>
            | None => ReasonReact.null
            }}
           <RouterLink href="/my-library"> {str("Back")} </RouterLink>
         </div>
         {response##my_episodes
          ->Belt.Array.map(LibraryTypes.toMyEpisode)
          ->Belt.Array.map(episode =>
              <MyEpisodeCard key={episode.id} episode podcastItunesId userId />
            )
          |> ReasonReact.array}
       </PageContainer>;
     }}
  </>;
};
