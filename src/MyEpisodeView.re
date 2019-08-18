open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make = (~episode: LibraryTypes.myEpisode, ~userId, ~podcastItunesId) => {
  let myEpisodesQuery =
    LibraryQueries.GetMyEpisodes.make(
      ~podcastId=episode.podcastId,
      ~userId,
      (),
    );

  let refetchQueries = [|Utils.toQueryObj(myEpisodesQuery)|];

  let (onRemove, removeResult) =
    UseRemoveEpisode.useRemoveEpisode(
      ~userId,
      ~episodeId=episode.id,
      ~refetchQueries,
      (),
    );

  <LibraryCard>
    <CardBody>
      <CardTitle> {str(episode.title)} </CardTitle>
      <LibraryCardSubtitle>
        {str(
           episode.pubDate ++ ", length: " ++ string_of_int(episode.lengthSec),
         )}
      </LibraryCardSubtitle>
      <CardText tag="div">
        <div
          dangerouslySetInnerHTML={
            "__html": Utils.truncateDescription(episode.description),
          }
        />
      </CardText>
      <BottomActions>
        {switch (episode.itunesId, podcastItunesId) {
         | (Some(episodeItunesId), Some(podcastId)) =>
           <NavLink
             href={Utils.makeEpisodeItunesUrl(
               ~podcastItunesId=podcastId,
               ~episodeItunesId,
             )}>
             {str("Open in itunes")}
           </NavLink>
         | (_, _) => ReasonReact.null
         }}
        <ActionButton
          disabled={removeResult === Loading}
          onClick=onRemove
          action=ActionButton.Remove
        />
      </BottomActions>
    </CardBody>
  </LibraryCard>;
};
