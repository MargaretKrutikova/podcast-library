open BsReactstrap;
open Cards;

let str = ReasonReact.string;

[@react.component]
let make =
    (
      ~id,
      ~numberOfSavedEpisodes=0,
      ~description,
      ~title,
      ~publisher,
      ~itunesId,
      ~userId,
    ) => {
  let hasSavedEpisodes = numberOfSavedEpisodes > 0;

  let myPodcastsQuery = LibraryQueries.GetAllPodcasts.make(~userId, ());
  let refetchQueries = [|Utils.toQueryObj(myPodcastsQuery)|];

  let (onRemove, removeResult) =
    UseRemovePodcast.useRemovePodcast(
      ~userId,
      ~podcastId=id,
      ~refetchQueries,
      (),
    );

  <LibraryCard hasTopActions=true>
    {hasSavedEpisodes
       ? <TopActions>
           <RouterLink href={"/my-library/" ++ id ++ "/episodes"}>
             {str("Show episodes")}
           </RouterLink>
         </TopActions>
       : ReasonReact.null}
    <CardBody>
      <CardTitle> {str(title)} </CardTitle>
      <LibraryCardSubtitle> {str(publisher)} </LibraryCardSubtitle>
      {hasSavedEpisodes
         ? <LibraryCardSubtitle>
             {str("Saved episodes: " ++ string_of_int(numberOfSavedEpisodes))}
           </LibraryCardSubtitle>
         : ReasonReact.null}
      <CardText tag="div">
        <div
          dangerouslySetInnerHTML={
            "__html": Utils.truncateDescription(description),
          }
        />
      </CardText>
      <BottomActions>
        <NavLink href={Utils.makePodcastItunesUrl(itunesId)}>
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
