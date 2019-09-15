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
    <MaterialUi_CardContent>
      <Cards.Title gutterBottom=true> {str(episode.title)} </Cards.Title>
      <MaterialUi_Typography gutterBottom=true variant=`Subtitle1>
        {str(
           episode.pubDate ++ ", length: " ++ string_of_int(episode.lengthSec),
         )}
      </MaterialUi_Typography>
      <MaterialUi_Typography
        variant=`Body1 color=`TextSecondary component={`String("div")}>
        <div
          dangerouslySetInnerHTML={
            "__html": Utils.truncateDescription(episode.description),
          }
        />
      </MaterialUi_Typography>
    </MaterialUi_CardContent>
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
