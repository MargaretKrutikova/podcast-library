open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make = (~podcast: MyLibrary.myPodcast) => {
  let hasSavedEpisodes = podcast.numberOfSavedEpisodes > 0;

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
        <Button size="sm" color="primary">
          //  disabled=isSaving
          //  onClick={_ => handlePodcastSave()}
           {str("Remove")} </Button>
      </BottomActions>
    </CardBody>
  </LibraryCard>;
};
