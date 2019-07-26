let str = ReasonReact.string;

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let myLibrary = AppCore.useSelector(model => model.myLibrary);

  React.useEffect0(() => {
    dispatch(FetchLibrary);
    None;
  });

  <>
    <h1> {str("My library")} </h1>
    {switch (myLibrary) {
     | Loaded(library)
     | Loading(Some(library)) =>
       <div>
         {library.myPodcasts
          ->Belt.Array.map(podcast =>
              <MyPodcastView key={podcast.listennotesId} podcast />
            )
          |> ReasonReact.array}
       </div>
     | _ => <div />
     }}
  </>;
};
