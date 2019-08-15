open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make = (~podcast: SearchTypes.podcast, ~isSaved) => {
  let dispatch = AppCore.useDispatch();
  let user = UserIdentity.useLoggedInUser();

  /** save */
  let handleSaveError = _ =>
    dispatch(ShowNotification({text: "Failed to save", type_: Danger}));

  let handleSaveDone = _ =>
    dispatch(ShowNotification({text: "Saved to library", type_: Info}));

  let save = (mutation: SavePodcast.Mutation.apolloMutation, userId) => {
    let refetchMyLibraryQuery = MyLibrary.makeGetMyLibraryQuery(~userId);
    let savePodcastMutation = SavePodcast.makeMutation(~podcast, ~userId);

    mutation(
      ~variables=savePodcastMutation##variables,
      ~refetchQueries=_ => [|Utils.toQueryObj(refetchMyLibraryQuery)|],
      ~update=SavePodcast.addPodcastIdToCache(~userId),
      (),
    )
    |> ignore;
  };

  let handleSave = (mutation: SavePodcast.Mutation.apolloMutation) => {
    switch (user) {
    | Anonymous => dispatch(OnUnauthorizedAccess)
    | LoggedIn(id) => save(mutation, id) |> ignore
    };
  };

  /** remove */
  let handleRemove = (mutation: RemoveContent.PodcastMutation.apolloMutation) => {
    switch (user) {
    | Anonymous => dispatch(OnUnauthorizedAccess)
    | LoggedIn(userId) =>
      RemoveContent.runPodcastMutation(
        ~mutation,
        ~podcastId=podcast.listennotesId,
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
      <CardTitle> {str(podcast.title)} </CardTitle>
      <LibraryCardSubtitle>
        {str(
           "Total: "
           ++ string_of_int(podcast.totalEpisodes)
           ++ ", last published: "
           ++ podcast.latestPubDate,
         )}
      </LibraryCardSubtitle>
      <LibraryCardSubtitle> {str(podcast.publisher)} </LibraryCardSubtitle>
      <CardText tag="div">
        <div
          dangerouslySetInnerHTML={
            "__html": Utils.truncateDescription(podcast.description),
          }
        />
      </CardText>
      <BottomActions>
        <NavLink
          href={Utils.makePodcastItunesUrl(
            string_of_int(podcast.podcastItunesId),
          )}>
          {str("Open in itunes")}
        </NavLink>
        {isSaved
           ? <RemoveContent.PodcastMutation
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
             </RemoveContent.PodcastMutation>
           : <SavePodcast.Mutation
               onError=handleSaveError onCompleted=handleSaveDone>
               ...{(mutation, {result}) =>
                 <Button
                   size="sm"
                   color="primary"
                   disabled={result == Loading}
                   onClick={_ => handleSave(mutation)}>
                   {str("Save")}
                 </Button>
               }
             </SavePodcast.Mutation>}
      </BottomActions>
    </CardBody>
  </SearchCard>;
};
