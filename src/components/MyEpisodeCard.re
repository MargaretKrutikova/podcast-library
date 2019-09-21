let str = ReasonReact.string;

[@react.component]
let make = (~episode: LibraryTypes.myEpisode, ~userId, ~podcastItunesId) => {
  let myEpisodesQuery = LibraryQueries.GetAllEpisodes.make(~userId, ());

  let refetchQueries = [|
    ReasonApolloHooks.Utils.toQueryObj(myEpisodesQuery),
  |];

  let (onRemove, removeResult) =
    UseRemoveEpisode.useRemoveEpisode(
      ~userId,
      ~episodeId=episode.id,
      ~refetchQueries,
      (),
    );

  <MaterialUi_Card>
    <Cards.EpisodeCardContent
      title={episode.title}
      podcastTitle={episode.podcastTitle}
      image={episode.image}
      description={episode.description}
      lengthSec={episode.lengthSec}
      pubDate={episode.pubDate}
      publisher={episode.publisher}
    />
    <Cards.CardActions>
      {switch (episode.itunesId, podcastItunesId) {
       | (Some(episodeItunesId), Some(podcastId)) =>
         <MaterialUi_Button
           color=`Secondary
           href={Utils.makeEpisodeItunesUrl(
             ~podcastItunesId=podcastId,
             ~episodeItunesId,
           )}>
           {str("Open in itunes")}
         </MaterialUi_Button>
       | (_, _) => ReasonReact.null
       }}
      <MaterialUi_Button
        size=`Small
        color=`Primary
        variant=`Contained
        disabled={removeResult === Loading}
        onClick={_ => onRemove() |> ignore}>
        {React.string("Remove")}
      </MaterialUi_Button>
    </Cards.CardActions>
  </MaterialUi_Card>;
};
