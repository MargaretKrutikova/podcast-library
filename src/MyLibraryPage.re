open BsReactstrap;

let str = ReasonReact.string;

let createItunesUrl = (podcastId, episodeId) => {
  "https://podcasts.apple.com/us/podcast/"
  ++ episodeId
  ++ "/id"
  ++ podcastId
  ++ "?i="
  ++ episodeId;
};

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let myLibrary = AppCore.useSelector(model => model.myLibrary);

  React.useEffect0(() => {
    dispatch(FetchLibrary);
    None;
  });

  let updateItunesId = (myEpisode: MyLibrary.myEpisode) => {
    SaveToLibrary.updateEpisodeItunesId(
      ~podcastItunesId=myEpisode.episode.podcastItunesId,
      ~episodeId=myEpisode.episodeId,
      ~episodeName=myEpisode.episode.title,
    )
    |> Js.Promise.(
         then_(affectedRows => {
           if (affectedRows > 0) {
             Js.log("YES");
           };
           resolve();
         })
       );
  };

  <Container>
    {str("My library")}
    {switch (myLibrary) {
     | Loaded(library)
     | Loading(Some(library)) =>
       library.episodes
       ->Belt.Array.map(episode =>
           <div key={episode.episodeId}>
             {str(episode.episode.title)}
             //  <p> {str(episode.episode.description)} </p>
             <p> {str("Published: " ++ episode.episode.pubDate)} </p>
             {switch (episode.episode.itunesId) {
              | None =>
                <Button onClick={_ => updateItunesId(episode)}>
                  {str("Update itunes id")}
                </Button>
              | Some(itunesId) =>
                <NavLink
                  href={createItunesUrl(
                    episode.episode.podcastItunesId,
                    itunesId,
                  )}>
                  {str("Listen on itunes")}
                </NavLink>
              }}
           </div>
         )
       |> ReasonReact.array
     | _ => <div />
     }}
  </Container>;
};
