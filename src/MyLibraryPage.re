let str = ReasonReact.string;
module GetMyLibraryQuery = ReasonApollo.CreateQuery(MyLibrary.GetMyLibrary);

[@react.component]
let make = (~userId) => {
  let myLibraryQuery = MyLibrary.GetMyLibrary.make(~userId, ());

  <>
    <h1> {str("My library")} </h1>
    <GetMyLibraryQuery variables=myLibraryQuery##variables>
      ...{({result}) =>
        switch (result) {
        | Loading => <div> {ReasonReact.string("Loading")} </div>
        | Data(response) =>
          <div>
            {MyLibrary.toMyLibrary(response).myPodcasts
             ->Belt.Array.map(podcast =>
                 <MyPodcastView key={podcast.listennotesId} podcast userId />
               )
             |> ReasonReact.array}
          </div>
        | Error(_) => <div> {ReasonReact.string("Error")} </div>
        }
      }
    </GetMyLibraryQuery>
  </>;
};
