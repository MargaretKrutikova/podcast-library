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
          onClick={_ => onRemove() |> ignore}
          action=ActionButton.Remove
        />
      : <ActionButton
          disabled={saveResult == Loading}
          onClick={_ => onSave() |> ignore}
          action=ActionButton.Save
        />;
  };
};

[@react.component]
let make = (~episode: SearchTypes.episode, ~isSaved) => {
  let dispatch = AppCore.useDispatch();
  let identity = UserIdentity.Context.useIdentityContext();

  let user = UserIdentity.getLoggedInUser(identity);
  let handleAnonymous = () => dispatch(OnUnauthorizedAccess);

  <Cards.SearchCard isSaved>
    <Cards.EpisodeCardContent
      title={episode.title}
      podcastTitle={episode.podcastTitle}
      image={episode.image}
      description={episode.description}
      lengthSec={episode.lengthSec}
      pubDate={episode.pubDate}
      publisher={episode.publisher}
    />
    <Cards.CardActions>
      <EpisodeItunesLink
        podcastItunesId={episode.podcastItunesId}
        episodeName={episode.title}
      />
      {switch (user) {
       | Anonymous =>
         <ActionButton
           onClick={_ => handleAnonymous() |> ignore}
           action=ActionButton.Save
         />
       | LoggedIn(userId) => <EpisodeLoggedInButton userId episode isSaved />
       }}
    </Cards.CardActions>
  </Cards.SearchCard>;
};
