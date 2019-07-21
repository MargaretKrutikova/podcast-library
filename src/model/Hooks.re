type searchQuery = {
  searchTerm: string,
  language: option(string),
  genreIds: array(int),
  offset: int,
  podcastId: option(string),
  searchType: ContentType.t,
  hasSearchResult: bool,
};

let hasSearchResult = (model: AppModel.model) =>
  switch (model.episodeSearchResult) {
  | NotAsked
  | Loading(None) => false
  | _ => true
  };

let useSearchQuery = () => {
  let baseQuery = AppCore.useSelector(model => model.baseSearchQuery);
  let episodeQuery = AppCore.useSelector(model => model.episodeSearchQuery);
  let searchType = AppCore.useSelector(model => model.searchContentType);
  let hasSearchResult = AppCore.useSelector(model => hasSearchResult(model));
  {
    searchTerm: baseQuery.searchTerm,
    language: baseQuery.language,
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
