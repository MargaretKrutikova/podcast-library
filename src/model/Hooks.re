type searchQuery = {
  searchTerm: string,
  genreIds: array(int),
  podcastId: option(string),
  searchType: ContentType.t,
};

let getSearchModel = (model: AppCore.model) => model.search;

let useSearchQuery = () => {
  let searchModel = AppCore.useSelector(getSearchModel);
  {
    searchTerm: searchModel.baseQuery.searchTerm,
    genreIds:
      Belt.Option.getWithDefault(searchModel.baseQuery.genreIds, [||]),
    podcastId: searchModel.episodeQuery.podcastId,
    searchType: searchModel.searchType,
  };
};

let useSearchDispatch = () => {
  let dispatch = AppCore.useDispatch();
  searchMessage => dispatch(GotSearchMessage(searchMessage));
};
