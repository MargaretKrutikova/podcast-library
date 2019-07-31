type searchQuery = {
  searchTerm: string,
  genreIds: array(int),
  podcastId: option(string),
  searchType: ContentType.t,
};

let useSearchQuery = () => {
  let baseQuery = AppCore.useSelector(model => model.search.baseQuery);
  let episodeQuery = AppCore.useSelector(model => model.search.episodeQuery);
  let searchType = AppCore.useSelector(model => model.search.searchType);

  {
    searchTerm: baseQuery.searchTerm,
    genreIds: Belt.Option.getWithDefault(baseQuery.genreIds, [||]),
    podcastId: episodeQuery.podcastId,
    searchType,
  };
};

let useSearchDispatch = () => {
  let dispatch = AppCore.useDispatch();
  searchMessage => dispatch(GotSearchMessage(searchMessage));
};
