open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make = (~podcast: MyLibrary.podcast) => {
  <LibraryCard hasTopActions=true>
    <TopActions> <CardLink> {str("Show episodes")} </CardLink> </TopActions>
    <CardBody>
      <CardTitle> {str(podcast.title)} </CardTitle>
      <LibraryCardSubtitle> {str(podcast.publisher)} </LibraryCardSubtitle>
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
        <Button size="sm" color="primary">
          //  disabled=isSaving
          //  onClick={_ => handlePodcastSave()}
           {str("Remove")} </Button>
      </BottomActions>
    </CardBody>
  </LibraryCard>;
};
