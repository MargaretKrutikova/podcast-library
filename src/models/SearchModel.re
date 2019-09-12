type t = {
  searchType: ContentType.t,
  baseQuery: SearchQuery.baseQuery,
  episodeQuery: SearchQuery.episodeQuery,
};

let createInitModel = () => {
  searchType: Episode,
  baseQuery: SearchQuery.createBaseQuery(),
  episodeQuery: SearchQuery.createEpisodeQuery(),
};

let updateSearchTerm = (searchTerm, model) => {
  ...model,
  baseQuery: {
    ...model.baseQuery,
    searchTerm,
  },
};
let updateEpisodeSearchQuery = (episodeQuery, model) => {
  ...model,
  episodeQuery,
};
let updateSearchType = (searchType, model) => {...model, searchType};

let getSearchQuery = (model): Routing.searchQuery => {
  query: model.baseQuery.searchTerm,
  content: model.searchType,
};
