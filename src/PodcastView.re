open BsReactstrap;
open Cards;

let str = ReasonReact.string;

module PodcastLoggedInButton = {
  [@react.component]
  let make = (~userId, ~podcast, ~isSaved) => {
    let (onSave, saveResult) =
      UseSavePodcast.useSavePodcast(~userId, ~podcast);

    let (onRemove, removeResult) =
      UseRemovePodcast.useRemovePodcast(
        ~userId,
        ~podcastId=podcast.listennotesId,
      );

    isSaved
      ? <ActionButton
          disabled={removeResult === Loading}
          onClick=onRemove
          action=ActionButton.Remove
        />
      : <ActionButton
          disabled={saveResult == Loading}
          onClick=onSave
          action=ActionButton.Save
        />;
  };
};

[@react.component]
let make = (~podcast: SearchTypes.podcast, ~isSaved) => {
  let dispatch = AppCore.useDispatch();
  let user = UserIdentity.useLoggedInUser();

  let handleAnonymous = () => dispatch(OnUnauthorizedAccess);

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
      <BottomActions>
        <NavLink
          href={Utils.makePodcastItunesUrl(
            string_of_int(podcast.podcastItunesId),
          )}>
          {str("Open in itunes")}
        </NavLink>
        {switch (user) {
         | Anonymous =>
           <ActionButton onClick=handleAnonymous action=ActionButton.Save />
         | LoggedIn(userId) =>
           <PodcastLoggedInButton userId podcast isSaved />
         }}
      </BottomActions>
    </CardBody>
  </SearchCard>;
};
