open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make = (~episode: SearchResult.episode) => {
  let isSaved = Hooks.useIsSavedEpisode(episode.listennotesId);
  let dispatch = AppCore.useDispatch();

  let (isSaving, setIsSaving) = React.useState(() => false);

  let handleEpisodeSave = () => {
    setIsSaving(_ => true);
    SaveToLibrary.saveEpisode(episode, {tags: "", status: NotListened})
    |> Js.Promise.(
         then_(_ => {
           setIsSaving(_ => false);
           dispatch(SavedEpisode(episode));
           dispatch(
             ShowNotification({text: "Saved to library", type_: Info}),
           )
           |> resolve;
         })
       )
    |> Js.Promise.(
         catch(_ => {
           setIsSaving(_ => false);
           dispatch(
             ShowNotification({text: "Failed to save", type_: Danger}),
           )
           |> resolve;
         })
       );
  };

  <SearchCard isSaved>
    <CardBody>
      <CardTitle> {str(episode.title)} </CardTitle>
      <LibraryCardSubtitle>
        {str(episode.podcastTitle ++ ", " ++ episode.pubDate)}
      </LibraryCardSubtitle>
      <CardText tag="div">
        <div
          dangerouslySetInnerHTML={
            "__html": Utils.truncateDescription(episode.description),
          }
        />
      </CardText>
      <SearchCardActions isSaved isSaving onSave=handleEpisodeSave>
        <EpisodeItunesLink
          podcastItunesId={episode.podcastItunesId}
          episodeName={episode.title}
        />
      </SearchCardActions>
    </CardBody>
  </SearchCard>;
};
