open BsReactstrap;

let str = ReasonReact.string;

[@react.component]
let make = (~episode: SearchResult.episode) => {
  let isSaved = Hooks.useIsSavedEpisode(episode.listennotesId);
  let dispatch = AppCore.useDispatch();

  let (isSaving, setIsSaving) = React.useState(() => false);

  let descriptionText =
    String.length(episode.description) < 200
      ? episode.description : episode.description ++ "...";

  let handleEpisodeSave = () => {
    setIsSaving(_ => true);
    SaveToLibrary.saveEpisode(episode, {tags: "", status: NotListened})
    |> Js.Promise.(
         then_(_ => {
           setIsSaving(_ => false);
           dispatch(SavedEpisode(episode)) |> resolve;
         })
       )
    |> Js.Promise.(catch(_ => setIsSaving(_ => false) |> resolve));
  };

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
      <CardTitle> {str(episode.title)} </CardTitle>
      <CardSubtitle className="search-result-subtitle">
        {str(episode.podcastTitle ++ ", " ++ episode.pubDate)}
      </CardSubtitle>
      <CardText tag="div">
        <div dangerouslySetInnerHTML={"__html": descriptionText} />
      </CardText>
      {!isSaved
         ? <Button
             size="sm"
             color="primary"
             className="search-result-save-button"
             disabled=isSaving
             onClick={_ => handleEpisodeSave()}>
             {str("Save")}
           </Button>
         : ReasonReact.null}
    </CardBody>
  </Card>;
};
