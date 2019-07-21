/** my library model */

type myLibrary =
  | NotAsked
  | Loading(option(MyLibrary.library))
  | Loaded(MyLibrary.library);

type message =
  | FetchLibraryIds
  | GotLibraryIds(array(string))
  | FetchLibrary
  | GotLibrary(MyLibrary.library)
  | SavedEpisode(EpisodeSearch.episode)
  | RequestedSearch
  | EnteredSearchTerm(string)
  | SetContentType(ContentType.t)
  | GotSearchResult(EpisodeSearch.searchResult)
  | GotSearchError;

/** search model */
type searchResult('a) =
  | NotAsked
  | Loading(option('a))
  | Success('a)
  | Error;

type model = {
  searchContentType: ContentType.t,
  baseSearchQuery: SearchQuery.baseQuery,
  episodeSearchQuery: SearchQuery.episodeQuery,
  episodeSearchResult: searchResult(EpisodeSearch.searchResult),
  librarySavedIds: array(string),
  myLibrary,
};

let createInitModel = () => {
  searchContentType: Episode,
  baseSearchQuery: SearchQuery.createBaseQuery(),
  episodeSearchQuery: SearchQuery.createEpisodeQuery(),
  episodeSearchResult: NotAsked,
  librarySavedIds: [||],
  myLibrary: NotAsked,
};

/** effects */

let search = (model, dispatch) => {
  let {baseSearchQuery, episodeSearchQuery} = model;
  EpisodeSearch.searchForEpisodes(baseSearchQuery, episodeSearchQuery)
  |> Js.Promise.(
       then_(result => dispatch(GotSearchResult(result)) |> resolve)
     )
  |> ignore;

  None;
};

let getLibraryIds = (model, dispatch) => {
  MyLibrary.getSavedIds()
  |> Js.Promise.(then_(ids => dispatch(GotLibraryIds(ids)) |> resolve))
  |> ignore;

  None;
};

let getMyLibrary = ((), dispatch) => {
  MyLibrary.getFull()
  |> Js.Promise.(
       then_(episodes =>
         dispatch(GotLibrary({episodes: episodes})) |> resolve
       )
     )
  |> ignore;

  None;
};

/** update */

let updateBaseSearchQuery = (model, baseSearchQuery) => {
  ...model,
  baseSearchQuery,
};

let updateEpisodeSearchQuery = (model, episodeSearchQuery) => {
  ...model,
  episodeSearchQuery,
};

let toLoading = (library: myLibrary): myLibrary => {
  switch (library) {
  | NotAsked => Loading(None)
  | Loaded(data) => Loading(Some(data))
  | other => other
  };
};

let update = (model, message) => {
  let {baseSearchQuery} = model;
  switch (message) {
  | EnteredSearchTerm(searchTerm) => (
      updateBaseSearchQuery(model, {...baseSearchQuery, searchTerm}),
      None,
    )
  | SetContentType(searchContentType) => (
      {...model, searchContentType},
      None,
    )
  | RequestedSearch =>
    String.trim(baseSearchQuery.searchTerm) != ""
      ? (
        {...model, episodeSearchResult: Loading(None)},
        Some(search(model)),
      )
      : (model, None)

  | GotSearchResult(result) => (
      {...model, episodeSearchResult: Success(result)},
      None,
    )
  | GotSearchError => ({...model, episodeSearchResult: Error}, None)
  | FetchLibraryIds => (model, Some(getLibraryIds()))
  | FetchLibrary => (
      {...model, myLibrary: toLoading(model.myLibrary)},
      Some(getMyLibrary()),
    )
  | GotLibrary(myLibrary) => (
      {...model, myLibrary: Loaded(myLibrary)},
      None,
    )
  | GotLibraryIds(ids) => ({...model, librarySavedIds: ids}, None)
  | SavedEpisode(episode) => (
      {
        ...model,
        librarySavedIds:
          Belt.Array.concat(
            model.librarySavedIds,
            [|episode.listennotesId|],
          ),
      },
      None,
    )
  };
};

let initialState = (createInitModel(), None);
