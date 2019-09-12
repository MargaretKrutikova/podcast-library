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
  let refetchQueries = [|
    ReasonApolloHooks.Utils.toQueryObj(myPodcastsQuery),
  |];

  let (onRemove, removeResult) =
    UseRemovePodcast.useRemovePodcast(
      ~userId,
      ~podcastId=id,
      ~refetchQueries,
      (),
    );

  <MaterialUi_Card>
    {hasSavedEpisodes
       ? <RouterLink href={"/my-library/" ++ id ++ "/episodes"}>
           {str("Show episodes")}
         </RouterLink>
       : ReasonReact.null}
    <MaterialUi_CardContent>
      <Cards.Title> {str(title)} </Cards.Title>
      <MaterialUi_Typography gutterBottom=true variant=`Subtitle1>
        {str(publisher)}
      </MaterialUi_Typography>
      {hasSavedEpisodes
         ? <MaterialUi_Typography gutterBottom=true variant=`Subtitle1>
             {str("Saved episodes: " ++ string_of_int(numberOfSavedEpisodes))}
           </MaterialUi_Typography>
         : ReasonReact.null}
      <Cards.Description description />
    </MaterialUi_CardContent>
    <Cards.CardActions>
      <MaterialUi_Button
        color=`Secondary href={Utils.makePodcastItunesUrl(itunesId)}>
        {str("Open in itunes")}
      </MaterialUi_Button>
      {!hasSavedEpisodes
         ? <ActionButton
             disabled={removeResult === Loading}
             onClick={_ => onRemove() |> ignore}
             action=ActionButton.Remove
           />
         : ReasonReact.null}
    </Cards.CardActions>
  </MaterialUi_Card>;
};
