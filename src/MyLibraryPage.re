module GetMyLibraryQuery =
  ReasonApolloHooks.Query.Make(LibraryQueries.GetMyLibrary);

[@react.component]
let make = (~userId) => {
  let getMyLibrary = LibraryQueries.GetMyLibrary.make(~userId, ());

  let (simple, _full) =
    GetMyLibraryQuery.use(
      ~variables=getMyLibrary##variables,
      ~fetchPolicy=NetworkOnly,
      (),
    );

  <>
    <h1> {React.string("My library")} </h1>
    {switch (simple) {
     | NoData => React.null
     | Loading => <div> {React.string("Loading")} </div>
     | Data(response) =>
       <div>
         {LibraryTypes.toMyLibrary(response).myPodcasts
          ->Belt.Array.map(podcast =>
              <MyPodcastView key={podcast.listennotesId} podcast userId />
            )
          |> React.array}
       </div>
     | Error(_) => <div> {React.string("Error")} </div>
     }}
  </>;
};
