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
    ItunesEpisode.updateItunesId(
      ~podcastItunesId=myEpisode.podcast.itunesId,
      ~episodeId=myEpisode.listennotesId,
      ~episodeName=myEpisode.title,
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

  <>
    {str("My library")}
    {switch (myLibrary) {
     | Loaded(library)
     | Loading(Some(library)) =>
       library.episodes
       ->Belt.Array.map(episode =>
           <div key={episode.listennotesId}>
             {str(episode.title)}
             //  <p> {str(episode.episode.description)} </p>
             <p> {str("Published: " ++ episode.pubDate)} </p>
             {switch (episode.itunesId) {
              | None =>
                <Button onClick={_ => updateItunesId(episode)}>
                  {str("Update itunes id")}
                </Button>
              | Some(itunesId) =>
                <NavLink
                  href={createItunesUrl(episode.podcast.itunesId, itunesId)}>
                  {str("Listen on itunes")}
                </NavLink>
              }}
           </div>
         )
       |> ReasonReact.array
     | _ => <div />
     }}
  </>;
};
