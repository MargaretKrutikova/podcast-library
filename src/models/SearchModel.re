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

let toSearchQuery = (model): SearchQs.t => {
  query: model.baseQuery.searchTerm,
  content: model.searchType,
};

let fromSearchQuery = (searchQuery: SearchQs.t) => {
  searchType: searchQuery.content,
  baseQuery: SearchQuery.createBaseQuery(~searchTerm=searchQuery.query, ()),
  episodeQuery: SearchQuery.createEpisodeQuery(),
};
