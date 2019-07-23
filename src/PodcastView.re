open BsReactstrap;

let str = ReasonReact.string;

[@react.component]
let make = (~podcast: SearchResult.podcast) => {
  // let dispatch = AppCore.useDispatch();

  // let (isSaving, setIsSaving) = React.useState(() => false);

  let descriptionText =
    String.length(podcast.description) < 200
      ? podcast.description : podcast.description ++ "...";

  <Card className="search-result-card">
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
      <Button size="sm" color="primary" className="search-result-save-button">
        {str("Save")}
      </Button>
    </CardBody>
  </Card>;
};
