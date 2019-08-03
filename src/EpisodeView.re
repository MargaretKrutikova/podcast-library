open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make = (~episode: SearchResult.episode, ~isSaved) => {
  let dispatch = AppCore.useDispatch();
  let (isFetchingInfo, setIsFetchingInfo) = React.useState(() => false);

  /** save */
  let handleSaveError = _ =>
    dispatch(ShowNotification({text: "Failed to save", type_: Danger}));

  let handleSaveDone = _ =>
    dispatch(ShowNotification({text: "Saved to library", type_: Info}));

  let handleSave = (mutation: SaveEpisode.Mutation.apolloMutation) => {
    setIsFetchingInfo(_ => true);

    SaveEpisode.getEpisodeInsertInfo(episode)
    |> Js.Promise.(
         then_(episodeInfo => {
           let saveEpisodeMutation =
             SaveEpisode.makeMutation(
               ~episode,
               ~libraryData={tags: "", status: NotListened},
               ~episodeInfo,
             );

           setIsFetchingInfo(_ => false);
           mutation(
             ~variables=saveEpisodeMutation##variables,
             ~update=
               SaveEpisode.addEpisodeToCache(episode.podcastListennotesId),
             (),
           )
           |> ignore;
           resolve();
         })
       )
    |> Js.Promise.(catch(_ => handleSaveError() |> resolve));
  };

  /** remove */
  let handleRemoveError = _ =>
    dispatch(ShowNotification({text: "Failed to remove", type_: Danger}));

  let handleRemoveDone = _ =>
    dispatch(ShowNotification({text: "Removed from library", type_: Info}));

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
      <BottomActions>
        <EpisodeItunesLink
          podcastItunesId={episode.podcastItunesId}
          episodeName={episode.title}
        />
        {isSaved
           ? <RemoveContent.EpisodeMutation
               onError=handleRemoveError onCompleted=handleRemoveDone>
               ...{(mutation, {result}) =>
                 <Button
                   size="sm"
                   color="warning"
                   disabled={result == Loading}
                   onClick={_ =>
                     RemoveContent.runEpisodeMutation(
                       ~mutation,
                       ~episodeId=episode.listennotesId,
                       ~podcastId=episode.podcastListennotesId,
                     )
                   }>
                   {str("Remove")}
                 </Button>
               }
             </RemoveContent.EpisodeMutation>
           : <SaveEpisode.Mutation
               onError=handleSaveError onCompleted=handleSaveDone>
               ...{(mutation, {result}) =>
                 <Button
                   size="sm"
                   color="primary"
                   disabled={result == Loading || isFetchingInfo}
                   onClick={_ => handleSave(mutation)}>
                   {str("Save")}
                 </Button>
               }
             </SaveEpisode.Mutation>}
      </BottomActions>
    </CardBody>
  </SearchCard>;
};
