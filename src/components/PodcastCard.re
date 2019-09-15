open Cards;

let str = ReasonReact.string;

module PodcastLoggedInButton = {
  [@react.component]
  let make = (~userId, ~podcast, ~isSaved) => {
    let (onSave, saveResult) =
      UseSavePodcast.useSavePodcast(~userId, ~podcast);

    let (onRemove, removeResult) =
      UseRemovePodcast.useRemovePodcast(~userId, ~podcastId=podcast.id, ());

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
let make = (~podcast: SearchTypes.podcast, ~isSaved) => {
  let dispatch = AppCore.useDispatch();
  let identity = UserIdentity.Context.useIdentityContext();

  let user = UserIdentity.getLoggedInUser(identity);

  let handleAnonymous = () => dispatch(OnUnauthorizedAccess);
  let setEpisodeQueryPodcast = () => {
    let podcastSearch: AppModel.podcastSearchQuery = {
      id: podcast.id,
      title: podcast.title,
    };
    dispatch(SetEpisodeQueryPodcast(podcastSearch));
  };

  <SearchCard isSaved>
    <MaterialUi_CardContent>
      <Cards.PodcastFilterButton onDelete=setEpisodeQueryPodcast />
      <Cards.Title> {str(podcast.title)} </Cards.Title>
      <MaterialUi_Typography variant=`Subtitle1>
        {str(podcast.publisher)}
      </MaterialUi_Typography>
      <MaterialUi_Typography gutterBottom=true variant=`Subtitle2>
        {str(
           "Total: "
           ++ string_of_int(podcast.totalEpisodes)
           ++ ", last published: "
           ++ podcast.latestPubDate,
         )}
      </MaterialUi_Typography>
      <Cards.Description description={podcast.description} />
    </MaterialUi_CardContent>
    <Cards.CardActions>
      <MaterialUi_Button
        color=`Secondary
        href={Utils.makePodcastItunesUrl(
          string_of_int(podcast.podcastItunesId),
        )}>
        {str("Open in itunes")}
      </MaterialUi_Button>
      {switch (user) {
       | Anonymous =>
         <ActionButton
           onClick={_ => handleAnonymous() |> ignore}
           action=ActionButton.Save
         />
       | LoggedIn(userId) => <PodcastLoggedInButton userId podcast isSaved />
       }}
    </Cards.CardActions>
  </SearchCard>;
};
