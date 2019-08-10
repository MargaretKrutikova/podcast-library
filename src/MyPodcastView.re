open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make = (~podcast: MyLibrary.myPodcast, ~userId) => {
  let hasSavedEpisodes = podcast.numberOfSavedEpisodes > 0;
  let dispatch = AppCore.useDispatch();

  let handleRemoveError = _ =>
    dispatch(ShowNotification({text: "Failed to remove", type_: Danger}));

  let handleRemoveDone = _ =>
    dispatch(ShowNotification({text: "Removed from library", type_: Info}));

  <LibraryCard hasTopActions=true>
    {hasSavedEpisodes
       ? <TopActions>
           <RouterLink
             href={"/my-library/" ++ podcast.listennotesId ++ "/episodes"}>
             {str("Show episodes")}
           </RouterLink>
         </TopActions>
       : ReasonReact.null}
    <CardBody>
      <CardTitle> {str(podcast.title)} </CardTitle>
      <LibraryCardSubtitle> {str(podcast.publisher)} </LibraryCardSubtitle>
      {hasSavedEpisodes
         ? <LibraryCardSubtitle>
             {str(
                "Saved episodes: "
                ++ string_of_int(podcast.numberOfSavedEpisodes),
              )}
           </LibraryCardSubtitle>
         : ReasonReact.null}
      <CardText tag="div">
        <div
          dangerouslySetInnerHTML={
            "__html": Utils.truncateDescription(podcast.description),
          }
        />
      </CardText>
      <BottomActions>
        <NavLink href={Utils.makePodcastItunesUrl(podcast.itunesId)}>
          {str("Open in itunes")}
        </NavLink>
        {!hasSavedEpisodes
           ? <RemoveContent.PodcastMutation
               onError=handleRemoveError onCompleted=handleRemoveDone>
               ...{(mutation, {result}) =>
                 <Button
                   size="sm"
                   color="warning"
                   disabled={result == Loading}
                   onClick={_ =>
                     RemoveContent.runPodcastMutation(
                       ~mutation,
                       ~podcastId=podcast.listennotesId,
                       ~userId,
                     )
                   }>
                   {str("Remove")}
                 </Button>
               }
             </RemoveContent.PodcastMutation>
           : ReasonReact.null}
      </BottomActions>
    </CardBody>
  </LibraryCard>;
};
