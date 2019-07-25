type previousQuery = SearchQuery.baseQuery;

type searchApiData('a) =
  | NotAsked
  | Loading(option('a), previousQuery)
  | FetchingMore('a, previousQuery)
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
  | Success(data, _)
  | FetchingMore(data, _) => Loading(Some(data), query)
  | other => other
  };

let getPreviousQuery = (data: searchApiData('a)) =>
  switch (data) {
  | Loading(_, query)
  | Success(_, query)
  | FetchingMore(_, query) => Some(query)
  | _ => None
  };

let hasSearchResult = (data: searchApiData('a)) =>
  switch (data) {
  | NotAsked
  | Loading(None, _) => false
  | _ => true
  };

let isFetchingMore = (data: searchApiData('a)) =>
  switch (data) {
  | FetchingMore(_, _) => true
  | _ => false
  };

let getData = data =>
  switch (data) {
  | Success(data, _)
  | FetchingMore(data, _)
  | Loading(Some(data), _) => Some(data)
  | _ => None
  };

let isEqual = (q1, q2) => q1 == q2;

let hasQueryChanged = (data: searchApiData('a), query) =>
  getPreviousQuery(data)
  ->Belt.Option.mapWithDefault(true, prev => !isEqual(query, prev));

type message =
  | RequestedSearch
  | EnteredSearchTerm(string)
  | SetContentType(ContentType.t)
  | RequestMoreResults
  | GotEpisodeSearchResult(SearchResult.episodeResults, previousQuery)
  | GotPodcastSearchResult(SearchResult.podcastResults, previousQuery);
//| GotSearchError;

/** effects */

let searchEpisodes = (model, offset, toGlobalMsg, dispatch) => {
  let {baseQuery, episodeQuery} = model;
  EpisodeSearch.searchForEpisodes(baseQuery, offset, episodeQuery)
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

let searchPodcasts = (model, offset, toGlobalMsg, dispatch) => {
  PodcastSearch.searchForPodcasts(model.baseQuery, offset)
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
  Some(searchEpisodes(model, 0, toGlobalMsg)),
);

let requestPodcastSearch = (model, toGlobalMsg) => (
  {...model, podcastResult: toLoading(model.podcastResult, model.baseQuery)},
  Some(searchPodcasts(model, 0, toGlobalMsg)),
);

let fetchMoreEpisodes = (model, offset, toGlobalMsg) => (
  {...model, episodeResult: toLoading(model.episodeResult, model.baseQuery)},
  Some(searchEpisodes(model, offset, toGlobalMsg)),
);

let fetchMorePodcasts = (model, offset, toGlobalMsg) => (
  {...model, podcastResult: toLoading(model.podcastResult, model.baseQuery)},
  Some(searchPodcasts(model, offset, toGlobalMsg)),
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

let requestMoreResults = (model, toGlobalMsg) => {
  switch (model.searchType) {
  | ContentType.Podcast =>
    switch (model.podcastResult) {
    | Success(data, _) => (
        {...model, podcastResult: FetchingMore(data, model.baseQuery)},
        Some(searchPodcasts(model, data.nextOffset, toGlobalMsg)),
      )
    | _ => (model, None)
    }
  | ContentType.Episode =>
    switch (model.episodeResult) {
    | Success(data, _) => (
        {...model, episodeResult: FetchingMore(data, model.baseQuery)},
        Some(searchEpisodes(model, data.nextOffset, toGlobalMsg)),
      )
    | _ => (model, None)
    }
  };
};

let setSearchResult =
    (currentResult, receivedData: SearchResult.searchResult('a), query) =>
  getData(currentResult)
  ->Belt.Option.mapWithDefault(
      Success(receivedData, query), (data: SearchResult.searchResult('a)) =>
      if (data.nextOffset < receivedData.nextOffset) {
        Success(
          {
            ...receivedData,
            results: Belt.Array.concat(data.results, receivedData.results),
          },
          query,
        );
      } else {
        Success(receivedData, query);
      }
    );

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
      (
        {
          ...model,
          episodeResult:
            setSearchResult(model.episodeResult, episodeResult, query),
        },
        None,
      );
    } else {
      (model, None);
    }
  | GotPodcastSearchResult(podcastResult, query) =>
    if (isEqual(model.baseQuery, query)) {
      (
        {
          ...model,
          podcastResult:
            setSearchResult(model.podcastResult, podcastResult, query),
        },
        None,
      );
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
  | RequestMoreResults => requestMoreResults(model, toGlobalMsg)
  };
