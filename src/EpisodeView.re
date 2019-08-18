open BsReactstrap;
open Cards;

let str = ReasonReact.string;

module EpisodeLoggedInButton = {
  [@react.component]
  let make = (~userId, ~episode, ~isSaved) => {
    let (onSave, saveResult) =
      UseSaveEpisode.useSaveEpisode(~userId, ~episode);

    let (onRemove, removeResult) =
      UseRemoveEpisode.useRemoveEpisode(
        ~userId,
        ~episodeId=episode.listennotesId,
        (),
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
let make = (~episode: SearchTypes.episode, ~isSaved) => {
  let dispatch = AppCore.useDispatch();
  let user = UserIdentity.useLoggedInUser();

  let handleAnonymous = () => dispatch(OnUnauthorizedAccess);

  <SearchCard isSaved>
    <CardBody>
      <CardTitle> {str(episode.title)} </CardTitle>
      <LibraryCardSubtitle>
        {str(episode.podcastTitle ++ ", " ++ episode.pubDate)}
      </LibraryCardSubtitle>
      <CardText tag="div">
        <div
          dangerouslySetInnerHTML={
            "__html": Utils.truncateDescription(episode.description),
          }
        />
      </CardText>
      <BottomActions>
        <EpisodeItunesLink
          podcastItunesId={episode.podcastItunesId}
          episodeName={episode.title}
        />
        {switch (user) {
         | Anonymous =>
           <ActionButton onClick=handleAnonymous action=ActionButton.Save />
         | LoggedIn(userId) =>
           <EpisodeLoggedInButton userId episode isSaved />
         }}
      </BottomActions>
    </CardBody>
  </SearchCard>;
};
