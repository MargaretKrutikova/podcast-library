type searchQuery = {
  searchTerm: string,
  genreIds: array(int),
  offset: int,
  podcastId: option(string),
  searchType: ContentType.t,
  hasSearchResult: bool,
};

let hasSearchResult = (model: AppModel.model) =>
  SearchModel.hasSearchResult(model.search.episodeResult)
  || SearchModel.hasSearchResult(model.search.podcastResult);

let useSearchQuery = () => {
  let baseQuery = AppCore.useSelector(model => model.search.baseQuery);
  let episodeQuery = AppCore.useSelector(model => model.search.episodeQuery);
  let searchType = AppCore.useSelector(model => model.search.searchType);
  let hasSearchResult = AppCore.useSelector(model => hasSearchResult(model));
  {
    searchTerm: baseQuery.searchTerm,
    genreIds: Belt.Option.getWithDefault(baseQuery.genreIds, [||]),
    offset: baseQuery.offset,
    podcastId: episodeQuery.podcastId,
    hasSearchResult,
    searchType,
  };
};

let isSavedEpisode = (model: AppModel.model, listennotesId) => {
  Belt.Array.some(model.librarySavedIds, id => id == listennotesId);
};

let useIsSavedEpisode = listennotesId => {
  let isSaved =
    AppCore.useSelector(model => isSavedEpisode(model, listennotesId));

  isSaved;
};

type searchResultByType =
  | PodcastResult(SearchModel.podcastResult)
  | EpisodeResult(SearchModel.episodeResult);

let useSearchResult = () => {
  let episodeSearchResult =
    AppCore.useSelector(model => model.search.episodeResult);
  let podcastSearchResult =
    AppCore.useSelector(model => model.search.podcastResult);
  let searchContentType =
    AppCore.useSelector(model => model.search.searchType);

  switch (searchContentType) {
  | Podcast => PodcastResult(podcastSearchResult)
  | Episode => EpisodeResult(episodeSearchResult)
  };
};

let useSearchDispatch = () => {
  let dispatch = AppCore.useDispatch();
  searchMessage => dispatch(GotSearchMessage(searchMessage));
};
