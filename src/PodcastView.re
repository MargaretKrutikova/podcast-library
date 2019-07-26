open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make = (~podcast: SearchResult.podcast) => {
  let dispatch = AppCore.useDispatch();
  let isSaved = Hooks.useIsSavedPodcast(podcast.listennotesId);

  let (isSaving, setIsSaving) = React.useState(() => false);

  let handlePodcastSave = () => {
    setIsSaving(_ => true);
    SaveToLibrary.savePodcast(podcast)
    |> Js.Promise.(
         then_(_ => {
           setIsSaving(_ => false);
           dispatch(SavedPodcast(podcast)) |> resolve;
         })
       )
    |> Js.Promise.(catch(_ => setIsSaving(_ => false) |> resolve));
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
      <SearchCardActions isSaved isSaving onSave=handlePodcastSave>
        <NavLink
          href={Utils.makePodcastItunesUrl(
            string_of_int(podcast.podcastItunesId),
          )}>
          {str("Open in itunes")}
        </NavLink>
      </SearchCardActions>
    </CardBody>
  </SearchCard>;
};
