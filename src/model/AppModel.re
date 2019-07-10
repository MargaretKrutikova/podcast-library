type message =
  | RequestedSearch
  | EnteredSearchTerm(string)
  | GotSearchResult(SearchResult.t)
  | GotSearchError;

type searchResult =
  | NotAsked
  | Loading(option(SearchResult.t))
  | Success(SearchResult.t)
  | Error;

type model = {
  searchTerm: string,
  searchResult,
};

let search = (model, dispatch) => {
  let query =
    SearchQuery.create(
      ~searchTerm=model.searchTerm,
      ~content=Episode(SearchQuery.EpisodeQuery.defaultValue()),
      ~searchFields=Some([|Title, Description|]),
      ~genreIds=Some([|143|]),
      (),
    );

  Js.Promise.(
    ListenNotesApi.search(query)
    |> then_(result =>
         switch (result) {
         | Belt.Result.Ok(searchResult) =>
           dispatch(GotSearchResult(searchResult)) |> resolve
         | Belt.Result.Error () => dispatch(GotSearchError) |> resolve
         }
       )
  )
  |> ignore;

  None;
};

let update = (model, message) => {
  switch (message) {
  | EnteredSearchTerm(searchTerm) => ({...model, searchTerm}, None)
  | RequestedSearch => (
      {...model, searchResult: Loading(None)},
      Some(search(model)),
    )
  | GotSearchResult(result) => (
      {...model, searchResult: Success(result)},
      None,
    )
  | GotSearchError => ({...model, searchResult: Error}, None)
  };
};

let initialState = ({searchTerm: "", searchResult: NotAsked}, None);
