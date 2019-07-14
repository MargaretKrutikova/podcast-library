open BsReactstrap;

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

  <div>
    <Form
      className="cr-search-form"
      onSubmit={e => ReactEvent.Form.preventDefault(e)}>
      <h1> {str("Search content")} </h1>
      <FormGroup>
        <Input
          _type="search"
          className="cr-search-form__input"
          placeholder="Search..."
          value={state.searchTerm}
          onChange=handleSearchChange
        />
      </FormGroup>
      <FormGroup tag="fieldset">
        <FormGroup check=true>
          <Label check=true>
            <input
              type_="radio"
              name="contentType"
              className="form-check-input"
              checked={state.contentType == Podcast}
              onChange={_ => dispatch(SetContentType(Podcast))}
            />
            {str("Podcast")}
          </Label>
        </FormGroup>
        <FormGroup check=true>
          <Label check=true>
            <input
              type_="radio"
              name="contentType"
              className="form-check-input"
              checked={state.contentType == Episode}
              onChange={_ => dispatch(SetContentType(Episode))}
            />
            {str("Episode")}
          </Label>
        </FormGroup>
      </FormGroup>
    </Form>
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
