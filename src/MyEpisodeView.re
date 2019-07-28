open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make = (~episode: MyLibrary.myEpisode, ~podcastItunesId) => {
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
        <Button size="sm" color="warning">
          //  disabled=isSaving
          //  onClick={_ => handlePodcastSave()}
           {str("Remove")} </Button>
      </BottomActions>
    </CardBody>
  </LibraryCard>;
};
