let str = ReasonReact.string;

module GetMyEpisodesQuery = ReasonApollo.CreateQuery(MyLibrary.GetMyEpisodes);

[@react.component]
let make = (~podcastId: string) => {
  let myEpisodesQuery =
    MyLibrary.GetMyEpisodes.make(~userId="margaretkru", ~podcastId, ());

  let handleEpisodeRemove =
      (mutation: RemoveContent.EpisodeMutation.apolloMutation, episodeId) => {
    let removeEpisodeMutation = RemoveContent.makeEpisodeMutation(~episodeId);

    mutation(
      ~variables=removeEpisodeMutation##variables,
      ~refetchQueries=_ => [|Utils.toQueryObj(myEpisodesQuery)|],
      ~update=RemoveContent.removeEpisodeFromCache(podcastId),
      (),
    )
    |> ignore;
  };

  <>
    <h1> {str("My episodes")} </h1>
    <GetMyEpisodesQuery
      variables=myEpisodesQuery##variables fetchPolicy="network-only">
      ...{({result}) =>
        switch (result) {
        | Loading => <div> {str("Loading")} </div>
        | Error(_) => <div> {str("Error")} </div>
        | Data(response) =>
          let podcast = response##podcasts->Belt.Array.get(0);
          let podcastItunesId =
            podcast->Belt.Option.mapWithDefault(None, data =>
              Some(data##itunesId)
            );

          <>
            <div>
              {switch (podcast) {
               | Some(data) => <h3> {str(data##title)} </h3>
               | None => ReasonReact.null
               }}
              <RouterLink href="/my-library"> {str("Back")} </RouterLink>
            </div>
            {response##my_episodes
             ->Belt.Array.map(MyLibrary.toMyEpisode)
             ->Belt.Array.map(episode =>
                 <MyEpisodeView
                   key={episode.listennotesId}
                   episode
                   podcastItunesId
                   onRemove=handleEpisodeRemove
                 />
               )
             |> ReasonReact.array}
          </>;
        }
      }
    </GetMyEpisodesQuery>
  </>;
};
