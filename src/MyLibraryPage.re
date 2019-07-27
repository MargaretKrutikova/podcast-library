let str = ReasonReact.string;
module GetMyLibraryQuery = ReasonApollo.CreateQuery(MyLibrary.GetMyLibrary);

[@react.component]
let make = () => {
  let myLibraryQuery =
    MyLibrary.GetMyLibrary.make(~user_id="margaretkru", ());

  <>
    <h1> {str("My library")} </h1>
    <GetMyLibraryQuery variables=myLibraryQuery##variables>
      ...{({result}) =>
        switch (result) {
        | Loading => <div> {ReasonReact.string("Loading")} </div>
        | Data(response) =>
          <div>
            {response##podcasts
             ->Belt.Array.map(podcast =>
                 <MyPodcastView key={podcast.listennotesId} podcast />
               )
             |> ReasonReact.array}
          </div>
        | Error(_) => <div> {ReasonReact.string("Error")} </div>
        }
      }
    </GetMyLibraryQuery>
  </>;
};
