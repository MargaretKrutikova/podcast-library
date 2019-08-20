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
  let user = UserIdentity.useLoggedInUser();

  let handleAnonymous = () => dispatch(OnUnauthorizedAccess);

  <Cards.SearchCard isSaved>
    <MaterialUi_CardContent>
      <MaterialUi_Typography gutterBottom=true variant=`H6>
        {str(episode.title)}
      </MaterialUi_Typography>
      <MaterialUi_Typography gutterBottom=true variant=`Subtitle1>
        {str(episode.podcastTitle ++ ", " ++ episode.pubDate)}
      </MaterialUi_Typography>
      <Cards.Description description={episode.description} />
    </MaterialUi_CardContent>
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
