type previousQuery = SearchQuery.baseQuery;

type searchApiData('a) =
  | NotAsked
  | Loading(option('a), previousQuery)
  | Success('a, previousQuery)
  | Error;

type episodeResult = searchApiData(SearchResult.episodeResults);
type podcastResult = searchApiData(SearchResult.podcastResults);

type t = {
  searchType: ContentType.t,
  baseQuery: SearchQuery.baseQuery,
  episodeQuery: SearchQuery.episodeQuery,
  episodeResult,
  podcastResult,
};

let createInitModel = () => {
  searchType: Episode,
  baseQuery: SearchQuery.createBaseQuery(),
  episodeQuery: SearchQuery.createEpisodeQuery(),
  episodeResult: NotAsked,
  podcastResult: NotAsked,
};

/** helpers */

let toLoading = (apiData, query) =>
  switch (apiData) {
  | NotAsked
  | Error => Loading(None, query)
  | Success(data, _) => Loading(Some(data), query)
  | other => other
  };

let getPreviousQuery = (data: searchApiData('a)) =>
  switch (data) {
  | Loading(_, query)
  | Success(_, query) => Some(query)
  | _ => None
  };

let hasSearchResult = (data: searchApiData('a)) =>
  switch (data) {
  | NotAsked
  | Loading(None, _) => false
  | _ => true
  };

let isEqual = (q1, q2) => q1 == q2;

let hasQueryChanged = (data: searchApiData('a), query) =>
  getPreviousQuery(data)
  ->Belt.Option.mapWithDefault(true, prev => !isEqual(query, prev));

type message =
  | RequestedSearch
  | EnteredSearchTerm(string)
  | SetContentType(ContentType.t)
  | GotEpisodeSearchResult(SearchResult.episodeResults, previousQuery)
  | GotPodcastSearchResult(SearchResult.podcastResults, previousQuery);
//| GotSearchError;

/** effects */

let searchEpisodes = (model, toGlobalMsg, dispatch) => {
  let {baseQuery, episodeQuery} = model;
  EpisodeSearch.searchForEpisodes(baseQuery, episodeQuery)
  |> Js.Promise.(
       then_(result =>
         dispatch(
           GotEpisodeSearchResult(result, model.baseQuery) |> toGlobalMsg,
         )
         |> resolve
       )
     )
  |> ignore;

  None;
};

let searchPodcasts = (model, toGlobalMsg, dispatch) => {
  PodcastSearch.searchForPodcasts(model.baseQuery)
  |> Js.Promise.(
       then_(result =>
         dispatch(
           toGlobalMsg(GotPodcastSearchResult(result, model.baseQuery)),
         )
         |> resolve
       )
     )
  |> ignore;

  None;
};

/** update */

let requestEpisodeSearch = (model, toGlobalMsg) => (
  {...model, episodeResult: toLoading(model.episodeResult, model.baseQuery)},
  Some(searchEpisodes(model, toGlobalMsg)),
);

let requestPodcastSearch = (model, toGlobalMsg) => (
  {...model, podcastResult: toLoading(model.podcastResult, model.baseQuery)},
  Some(searchPodcasts(model, toGlobalMsg)),
);

let searchByType = (model, toGlobalMsg) => {
  switch (model.searchType) {
  | ContentType.Podcast =>
    if (hasQueryChanged(model.podcastResult, model.baseQuery)) {
      requestPodcastSearch(model, toGlobalMsg);
    } else {
      (model, None);
    }
  | ContentType.Episode =>
    if (hasQueryChanged(model.episodeResult, model.baseQuery)) {
      requestEpisodeSearch(model, toGlobalMsg);
    } else {
      (model, None);
    }
  };
};

let updateBaseSearchQuery = (model, baseQuery) => {...model, baseQuery};

let updateEpisodeSearchQuery = (model, episodeQuery) => {
  ...model,
  episodeQuery,
};

let update = (model, message, toGlobalMsg) =>
  switch (message) {
  | EnteredSearchTerm(searchTerm) => (
      updateBaseSearchQuery(model, {...model.baseQuery, searchTerm}),
      None,
    )
  | GotEpisodeSearchResult(episodeResult, query) =>
    if (isEqual(model.baseQuery, query)) {
      ({...model, episodeResult: Success(episodeResult, query)}, None);
    } else {
      (model, None);
    }
  | GotPodcastSearchResult(podcastResult, query) =>
    if (isEqual(model.baseQuery, query)) {
      ({...model, podcastResult: Success(podcastResult, query)}, None);
    } else {
      (model, None);
    }
  | RequestedSearch => searchByType(model, toGlobalMsg)
  | SetContentType(searchType) =>
    if (model.searchType == searchType) {
      (model, None);
    } else {
      searchByType({...model, searchType}, toGlobalMsg);
    }
  };
