let getSearchTerm = (model: AppModel.model) => model.search.searchTerm;

let getSearchType = (model: AppModel.model) => model.search.searchType;

let getSearchPodcast = (model: AppModel.model) => model.searchPodcast;

let getSearchModel = (model: AppModel.model) => model.search;

let hasSearchTerm = (model: AppModel.model) =>
  getSearchTerm(model)->Js.String.length > 0;
