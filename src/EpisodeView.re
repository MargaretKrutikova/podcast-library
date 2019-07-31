open BsReactstrap;
open Cards;

let str = ReasonReact.string;

module SaveEpisodeMutation =
  ReasonApollo.CreateMutation(SaveToLibrary.SaveEpisode);

[@react.component]
let make = (~episode: SearchResult.episode, ~isSaved) => {
  let dispatch = AppCore.useDispatch();
  let (isFetchingInfo, setIsFetchingInfo) = React.useState(() => false);

  let handleError = _ =>
    dispatch(ShowNotification({text: "Failed to save", type_: Danger}));

  let handleSaveDone = _ =>
    dispatch(ShowNotification({text: "Saved to library", type_: Info}));

  <SaveEpisodeMutation onError=handleError onCompleted=handleSaveDone>
    ...{(mutation, {result}) => {
      let refetchMyLibraryQuery = MyLibrary.getMyLibraryQuery();

      let handleSave = _ => {
        setIsFetchingInfo(_ => true);

        SaveToLibrary.getEpisodeInsertInfo(episode)
        |> Js.Promise.(
             then_(episodeInfo => {
               let saveEpisodeMutation =
                 SaveToLibrary.makeSaveEpisodeMutation(
                   ~episode,
                   ~libraryData={tags: "", status: NotListened},
                   ~episodeInfo,
                   (),
                 );

               setIsFetchingInfo(_ => false);
               mutation(
                 ~variables=saveEpisodeMutation##variables,
                 ~refetchQueries=
                   _ => [|Utils.toQueryObj(refetchMyLibraryQuery)|],
                 ~update=MyLibrary.addEpisodeIdToCache,
                 (),
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
          <BottomActions>
            <EpisodeItunesLink
              podcastItunesId={episode.podcastItunesId}
              episodeName={episode.title}
            />
            {!isSaved
               ? <Button
                   size="sm"
                   color="primary"
                   disabled={result == Loading || isFetchingInfo}
                   onClick=handleSave>
                   {str("Save")}
                 </Button>
               : <Button size="sm" color="warning"> {str("Remove")} </Button>}
          </BottomActions>
        </CardBody>
      </SearchCard>;
    }}
  </SaveEpisodeMutation>;
};
