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
      ~image,
      ~userId,
      ~addedDate,
    ) => {
  let hasSavedEpisodes = numberOfSavedEpisodes > 0;
  let dispatch = AppCore.useDispatch();

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

  let setEpisodeQueryPodcast = () => {
    let podcastSearch: AppModel.podcastSearchQuery = {id, title};
    dispatch(SetEpisodeQueryPodcast(podcastSearch));
  };

  let info =
    <>
      <Cards.Publisher publisher />
      <MaterialUi_Typography gutterBottom=true variant=`Subtitle2>
        {str(
           "Added: "
           ++ Js.Date.toLocaleDateString(
                Js.Json.decodeString(addedDate)
                ->Belt.Option.getWithDefault("")
                |> Js.Date.fromString,
              ),
         )}
      </MaterialUi_Typography>
      {hasSavedEpisodes
         ? <MaterialUi_Typography gutterBottom=true variant=`Subtitle1>
             {str("Saved episodes: " ++ string_of_int(numberOfSavedEpisodes))}
           </MaterialUi_Typography>
         : ReasonReact.null}
    </>;

  <MaterialUi_Card className=Css.(style([position(`relative)]))>
    {hasSavedEpisodes
       ? <RouterLink href={"/my-library/" ++ id ++ "/episodes"}>
           {str("Show episodes")}
         </RouterLink>
       : ReasonReact.null}
    <Cards.PodcastCardContent
      title
      image
      description
      onFilter=setEpisodeQueryPodcast
      info
    />
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
