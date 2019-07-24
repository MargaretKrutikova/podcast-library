open BsReactstrap;

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

  let descriptionText =
    String.length(podcast.description) < 200
      ? podcast.description : podcast.description ++ "...";

  <Card
    className={
      isSaved
        ? "search-result-card search-result-card--saved" : "search-result-card"
    }>
    {isSaved
       ? <Badge className="search-result-saved-badge" color="info">
           {str("Saved")}
         </Badge>
       : ReasonReact.null}
    <CardBody>
      <CardTitle> {str(podcast.title)} </CardTitle>
      <CardSubtitle className="search-result-subtitle">
        {str(
           "Total: "
           ++ string_of_int(podcast.totalEpisodes)
           ++ ", last published: "
           ++ podcast.latestPubDate,
         )}
      </CardSubtitle>
      <CardSubtitle className="search-result-subtitle">
        {str(podcast.publisher)}
      </CardSubtitle>
      <CardText tag="div">
        <div dangerouslySetInnerHTML={"__html": descriptionText} />
      </CardText>
      <div className="search-result-card-actions">
        <NavLink
          href={ItunesLink.makeForPodcast(
            string_of_int(podcast.podcastItunesId),
          )}>
          {str("Open in itunes")}
        </NavLink>
        {!isSaved
           ? <Button
               size="sm"
               color="primary"
               disabled=isSaving
               onClick={_ => handlePodcastSave()}>
               {str("Save")}
             </Button>
           : ReasonReact.null}
      </div>
    </CardBody>
  </Card>;
};
