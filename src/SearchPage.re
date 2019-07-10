let str = ReasonReact.string;

type state = {
  searchTerm: string,
  searchResult: AppModel.searchResult,
};

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let state =
    AppCore.useSelector(model =>
      {searchResult: model.searchResult, searchTerm: model.searchTerm}
    );

  let handleSearchChange = e =>
    dispatch(EnteredSearchTerm(ReactEvent.Form.target(e)##value));

  <div>
    <h1> {str("Search content")} </h1>
    <input value={state.searchTerm} onChange=handleSearchChange />
    <button onClick={_ => dispatch(RequestedSearch)}>
      {str("Click")}
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
               </div>
             }
           )
         |> ReasonReact.array
       | Error => <div> {str("Error")} </div>
       }}
    </div>
  </div>;
};
