let str = ReasonReact.string;

type state = {
  searchTerm: string,
  searchResult: AppModel.searchResult,
};

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let state = AppCore.useSelector(model => model);

  let handleSearchChange = e =>
    dispatch(EnteredSearchTerm(ReactEvent.Form.target(e)##value));

  let contentTypeValue =
    switch (state.contentType) {
    | Podcast => "podcast"
    | Episode => "episode"
    };
  Js.log(contentTypeValue);
  <div>
    <h1> {str("Search content")} </h1>
    <input value={state.searchTerm} onChange=handleSearchChange />
    <div>
      <label>
        <input
          type_="radio"
          name="contentType"
          checked={state.contentType == Podcast}
          onChange={_ => dispatch(SetContentType(Podcast))}
        />
        {str("Podcast")}
      </label>
      <label>
        <input
          type_="radio"
          name="contentType"
          checked={state.contentType == Episode}
          onChange={_ => dispatch(SetContentType(Episode))}
        />
        {str("Episode")}
      </label>
    </div>
    <button onClick={_ => dispatch(RequestedSearch)}>
      {str("Search")}
    </button>
    <div>
      {switch (state.searchResult) {
       | NotAsked => <span> {str("Nothing yet")} </span>
       | Loading(_) => <span> {str("Loading")} </span>
       | Success(res) =>
         res.results
         ->Belt.Array.map(result =>
             switch (result) {
             | Podcast(podcast) =>
               <div key={podcast.id}>
                 {str(podcast.title)}
                 <p> {str(podcast.description)} </p>
               </div>
             | Episode(episode) =>
               <div key={episode.id}>
                 {str(episode.title)}
                 <p> {str(episode.description)} </p>
                 <p>
                   {str(
                      "Published: "
                      ++ (
                        episode.pubDateMs
                        |> float_of_int
                        |> Js.Date.fromFloat
                        |> Js.Date.toLocaleDateString
                      ),
                    )}
                 </p>
               </div>
             }
           )
         |> ReasonReact.array
       | Error => <div> {str("Error")} </div>
       }}
    </div>
  </div>;
};
