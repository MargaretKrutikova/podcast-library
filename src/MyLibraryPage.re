let str = ReasonReact.string;

module GetMyLibraryQuery =
  ReasonApolloHooks.Query.Make(LibraryQueries.GetMyLibrary);

[@react.component]
let make = (~userId) => {
  let getMyEpisodes = LibraryQueries.GetMyLibrary.make(~userId, ());

  let (simple, _full) =
    GetMyLibraryQuery.use(~variables=getMyEpisodes##variables, ());

  <>
    <h1> {str("My library")} </h1>
    {switch (simple) {
     | NoData => React.null

     | Loading => <div> {ReasonReact.string("Loading")} </div>
     | Data(response) =>
       <div>
         {LibraryTypes.toMyLibrary(response).myPodcasts
          ->Belt.Array.map(podcast =>
              <MyPodcastView key={podcast.listennotesId} podcast userId />
            )
          |> ReasonReact.array}
       </div>
     | Error(_) => <div> {ReasonReact.string("Error")} </div>
     }}
  </>;
};
