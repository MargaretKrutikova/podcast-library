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

let updateSearchTerm = (model, searchTerm) => {
  ...model,
  baseQuery: {
    ...model.baseQuery,
    searchTerm,
  },
};
let updateEpisodeSearchQuery = (model, episodeQuery) => {
  ...model,
  episodeQuery,
};
let updateSearchType = (model, searchType) => {...model, searchType};
