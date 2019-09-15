let getSearchTerm = (model: AppCore.model) => model.search.searchTerm;

let getSearchType = (model: AppCore.model) => model.search.searchType;

let getSearchPodcast = (model: AppCore.model) => model.searchPodcast;

let getSearchModel = (model: AppCore.model) => model.search;

let hasSearchTerm = (model: AppCore.model) =>
  getSearchTerm(model)->Js.String.length > 0;
