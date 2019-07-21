/** my library model */
type savedEpisode =
  | Partial(MyLibrary.myPartialEpisode)
  | Full(MyLibrary.myEpisode);

type myLibrary = {episodes: array(savedEpisode)};

type message =
  | FetchPartialLibrary
  | FetchFullLibrary
  | GotPartialLibrary(MyLibrary.myLibraryPartial)
  | GotFullLibrary(MyLibrary.myLibraryFull)
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
  myLibrary,
};

let createInitModel = () => {
  searchContentType: Episode,
  baseSearchQuery: SearchQuery.createBaseQuery(),
  episodeSearchQuery: SearchQuery.createEpisodeQuery(),
  episodeSearchResult: NotAsked,
  myLibrary: {
    episodes: [||],
  },
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

let getMyLibraryPartial = ((), dispatch) => {
  MyLibrary.getPartial()
  |> Js.Promise.(
       then_(episodes =>
         dispatch(GotPartialLibrary({episodes: episodes})) |> resolve
       )
     )
  |> ignore;

  None;
};

let getMyLibraryFull = ((), dispatch) => {
  MyLibrary.getFull()
  |> Js.Promise.(
       then_(episodes =>
         dispatch(GotFullLibrary({episodes: episodes})) |> resolve
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

let mapFromPartialLibrary = ({episodes}: MyLibrary.myLibraryPartial) => {
  episodes: Belt.Array.map(episodes, ep => Partial(ep)),
};

let mapFromFullLibrary = ({episodes}: MyLibrary.myLibraryFull) => {
  episodes: Belt.Array.map(episodes, ep => Full(ep)),
};

let saveToLibrary = (library, episode: EpisodeSearch.episode) => {
  let partialEpisode: MyLibrary.myPartialEpisode = {
    status: NotListened,
    episode: {
      listennotesId: episode.listennotesId,
    },
  };
  {
    episodes:
      Belt.Array.concat(library.episodes, [|Partial(partialEpisode)|]),
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
  | FetchPartialLibrary => (model, Some(getMyLibraryPartial()))
  | FetchFullLibrary => (model, Some(getMyLibraryFull()))
  | GotPartialLibrary(library) => (
      {...model, myLibrary: mapFromPartialLibrary(library)},
      None,
    )
  | GotFullLibrary(library) => (
      {...model, myLibrary: mapFromFullLibrary(library)},
      None,
    )
  | SavedEpisode(episode) => (
      {...model, myLibrary: saveToLibrary(model.myLibrary, episode)},
      None,
    )
  };
};

let initialState = (createInitModel(), None);
