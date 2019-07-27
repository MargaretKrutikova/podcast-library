open BsReactstrap;
open Cards;

let str = ReasonReact.string;

module SavePodcastMutation =
  ReasonApollo.CreateMutation(SaveToLibrary.SavePodcast);

[@react.component]
let make = (~podcast: SearchResult.podcast) => {
  let dispatch = AppCore.useDispatch();
  let isSaved = Hooks.useIsSavedPodcast(podcast.listennotesId);

  <>
    <SavePodcastMutation>
      ...{(mutation, {result}) => {
        let savePodcastMutation = SaveToLibrary.savePodcastMutation(podcast);
        let refetchMyLibraryQuery = MyLibrary.getMyLibraryQuery();

        let handleSaveDone = (_, _result) =>
          dispatch(SavedPodcast(podcast));

        let handleSave = _ =>
          mutation(
            ~variables=savePodcastMutation##variables,
            ~refetchQueries=
              _ => [|Utils.convertToQueryObj(refetchMyLibraryQuery)|],
            ~update=handleSaveDone,
            (),
          );

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
            <LibraryCardSubtitle>
              {str(podcast.publisher)}
            </LibraryCardSubtitle>
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
              {!isSaved
                 ? <Button
                     size="sm"
                     color="primary"
                     disabled={result == Loading}
                     onClick=handleSave>
                     {str("Save")}
                   </Button>
                 : <Button size="sm" color="warning">
                     {str("Remove")}
                   </Button>}
            </BottomActions>
          </CardBody>
        </SearchCard>;
      }}
    </SavePodcastMutation>
  </>;
};
