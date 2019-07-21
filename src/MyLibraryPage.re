open BsReactstrap;

let str = ReasonReact.string;

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let myLibrary = AppCore.useSelector(model => model.myLibrary);

  React.useEffect0(() => {
    dispatch(FetchLibrary);
    None;
  });

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
           </div>
         )
       |> ReasonReact.array
     | _ => <div />
     }}
  </Container>;
};
