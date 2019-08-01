open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make = (~podcast: SearchResult.podcast, ~isSaved) => {
  let dispatch = AppCore.useDispatch();

  /** save */
  let handleSaveError = _ =>
    dispatch(ShowNotification({text: "Failed to save", type_: Danger}));

  let handleSaveDone = _ =>
    dispatch(ShowNotification({text: "Saved to library", type_: Info}));

  let handleSave = (mutation: SavePodcast.Mutation.apolloMutation) => {
    let refetchMyLibraryQuery = MyLibrary.getMyLibraryQuery();
    let savePodcastMutation = SavePodcast.makeMutation(podcast);

    mutation(
      ~variables=savePodcastMutation##variables,
      ~refetchQueries=_ => [|Utils.toQueryObj(refetchMyLibraryQuery)|],
      ~update=MyLibrary.addPodcastIdToCache,
      (),
    )
    |> ignore;
  };

  /** remove */
  let handleRemoveError = _ =>
    dispatch(ShowNotification({text: "Failed to remove", type_: Danger}));

  let handleRemoveDone = _ =>
    dispatch(ShowNotification({text: "Removed from library", type_: Info}));

  let handleRemove = (mutation: RemoveContent.PodcastMutation.apolloMutation) => {
    let refetchMyLibraryQuery = MyLibrary.getMyLibraryQuery();
    let removeEpisodeMutation =
      RemoveContent.makePodcastMutation(~podcastId=podcast.listennotesId);
    mutation(
      ~variables=removeEpisodeMutation##variables,
      ~refetchQueries=_ => [|Utils.toQueryObj(refetchMyLibraryQuery)|],
      ~update=MyLibrary.removePodcastIdFromCache,
      (),
    )
    |> ignore;
  };

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
