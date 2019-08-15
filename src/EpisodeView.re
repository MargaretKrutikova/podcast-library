open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make = (~episode: SearchTypes.episode, ~isSaved) => {
  let dispatch = AppCore.useDispatch();
  let user = UserIdentity.useLoggedInUser();
  let (isFetchingInfo, setIsFetchingInfo) = React.useState(() => false);

  /** save */
  let handleSaveError = _ =>
    dispatch(ShowNotification({text: "Failed to save", type_: Danger}));

  let handleSaveDone = _ =>
    dispatch(ShowNotification({text: "Saved to library", type_: Info}));

  // TODO: make saving a hook?
  let save = (mutation: SaveEpisode.Mutation.apolloMutation, userId) => {
    setIsFetchingInfo(_ => true);

    SaveEpisode.getEpisodeInsertInfo(episode)
    |> Js.Promise.(
         then_(episodeInfo => {
           let refetchMyLibraryQuery =
             MyLibrary.GetMyLibrary.make(~userId, ());

           let saveEpisodeMutation =
             SaveEpisode.makeMutation(
               ~userId,
               ~episode,
               ~libraryData={tags: "", status: NotListened},
               ~episodeInfo,
             );

           setIsFetchingInfo(_ => false);
           mutation(
             ~variables=saveEpisodeMutation##variables,
             ~update=
               SaveEpisode.addEpisodeToCache(
                 ~podcastId=episode.podcastListennotesId,
                 ~userId,
               ),
             ~refetchQueries=
               _ => [|Utils.toQueryObj(refetchMyLibraryQuery)|],
             (),
           )
           |> ignore;
           resolve();
         })
       )
    |> Js.Promise.(catch(_ => handleSaveError() |> resolve));
  };

  let handleSave = (mutation: SaveEpisode.Mutation.apolloMutation) => {
    switch (user) {
    | Anonymous => dispatch(OnUnauthorizedAccess)
    | LoggedIn(id) => save(mutation, id) |> ignore
    };
  };

  /** remove */
  let handleRemove = (mutation: RemoveContent.EpisodeMutation.apolloMutation) => {
    switch (user) {
    | Anonymous => dispatch(OnUnauthorizedAccess)
    | LoggedIn(userId) =>
      RemoveContent.runEpisodeMutation(
        ~mutation,
        ~episodeId=episode.listennotesId,
        ~podcastId=episode.podcastListennotesId,
        ~userId,
      )
      |> ignore
    };
  };

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
                   onClick={_ => handleRemove(mutation)}>
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
