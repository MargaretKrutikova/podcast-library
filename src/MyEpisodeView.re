open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make = (~episode: MyLibrary.myEpisode, ~podcastItunesId, ~onRemove) => {
  let dispatch = AppCore.useDispatch();

  let handleRemoveError = _ =>
    dispatch(ShowNotification({text: "Failed to remove", type_: Danger}));

  let handleRemoveDone = _ =>
    dispatch(ShowNotification({text: "Removed from library", type_: Info}));

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
        <RemoveContent.EpisodeMutation
          onError=handleRemoveError onCompleted=handleRemoveDone>
          ...{(mutation, {result}) =>
            <Button
              size="sm"
              color="warning"
              disabled={result == Loading}
              onClick={_ => onRemove(mutation, episode.listennotesId)}>
              {str("Remove")}
            </Button>
          }
        </RemoveContent.EpisodeMutation>
      </BottomActions>
    </CardBody>
  </LibraryCard>;
};
