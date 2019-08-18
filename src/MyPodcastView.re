open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make = (~podcast: LibraryTypes.myPodcast, ~userId) => {
  let hasSavedEpisodes = podcast.numberOfSavedEpisodes > 0;
  let (onRemove, removeResult) =
    UseRemovePodcast.useRemovePodcast(
      ~userId,
      ~podcastId=podcast.listennotesId,
    );

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
           ? <ActionButton
               disabled={removeResult === Loading}
               onClick=onRemove
               action=ActionButton.Remove
             />
           : ReasonReact.null}
      </BottomActions>
    </CardBody>
  </LibraryCard>;
};
