open BsReactstrap;
open ContentType;

let str = ReasonReact.string;

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let searchParams = AppCore.useSelector(model => model.searchParams);
  let searchResult = AppCore.useSelector(model => model.episodeSearchResult);

  React.useEffect0(() => {
    dispatch(FetchPartialLibrary);
    None;
  });

  let handleSearchChange = e =>
    dispatch(EnteredSearchTerm(ReactEvent.Form.target(e)##value));

  let handleEpisodeSave = (episode: EpisodeResult.episode) =>
    Mutations.saveEpisode(episode, {tags: "Elm, Blah", status: NotListened})
    |> Graphql.sendQuery
    |> Js.Promise.then_(response =>
         Js.log(response##insert_episodes) |> Js.Promise.resolve
       );

  <Container>
    <Form
      className="cr-search-form"
      onSubmit={e => ReactEvent.Form.preventDefault(e)}>
      <h1> {str("Search library")} </h1>
      <FormGroup>
        <Input
          _type="search"
          className="cr-search-form__input"
          placeholder="Search..."
          value={searchParams.searchTerm}
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
              checked={searchParams.contentType == Podcast}
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
              checked={searchParams.contentType == Episode}
              onChange={_ => dispatch(SetContentType(Episode))}
            />
            {str("Episode")}
          </Label>
        </FormGroup>
      </FormGroup>
    </Form>
    <Button onClick={_ => dispatch(RequestedSearch)}>
      {str("Search")}
    </Button>
    <div>
      {switch (searchResult) {
       | NotAsked => <span> {str("Nothing yet")} </span>
       | Loading(_) => <span> {str("Loading")} </span>
       | Success(res) =>
         res.results
         ->Belt.Array.map(episode
             //  switch (result) {
             //  | Podcast(podcast) =>
             //    <div key={podcast.id}>
             //      {str(podcast.title)}
             //      <p> {str(podcast.description)} </p>
             //    </div>
             //  | Episode(episode) =>
             =>
               <div key={episode.listennotesId}>
                 {str(episode.title)}
                 <p> {str(episode.description)} </p>
                 <p>
                   <Button
                     onClick={_ => handleEpisodeSave(episode) |> ignore}>
                     {str("Save")}
                   </Button>
                 </p>
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
             )
         // }
         |> ReasonReact.array
       | Error => <div> {str("Error")} </div>
       }}
    </div>
  </Container>;
};
