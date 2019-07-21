open BsReactstrap;

let str = ReasonReact.string;

[@react.component]
let make =
    (
      ~title,
      ~podcastTitle,
      ~listennotesId,
      ~pubDate,
      ~lengthSec=?,
      ~description,
    ) => {
  let isSaved = Hooks.useIsSavedEpisode(listennotesId);

  let descriptionText =
    String.length(description) < 200 ? description : description ++ "...";

  let handleEpisodeSave = (episode: EpisodeSearch.episode) =>
    SaveToLibrary.saveEpisode(
      episode,
      {tags: "Elm, Blah", status: NotListened},
    )
    |> Graphql.sendQuery
    |> Js.Promise.then_(response =>
         Js.log(response##insert_episodes) |> Js.Promise.resolve
       );

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
      <CardTitle> {str(title)} </CardTitle>
      <CardSubtitle className="search-result-subtitle">
        {str(podcastTitle ++ ", " ++ pubDate)}
      </CardSubtitle>
      <CardText tag="div">
        <div dangerouslySetInnerHTML={"__html": descriptionText} />
      </CardText>
      {!isSaved
         ? <Button
             size="sm"
             color="primary"
             className="search-result-save-button"
             onClick=handleEpisodeSave>
             {str("Save")}
           </Button>
         : ReasonReact.null}
    </CardBody>
  </Card>;
};
