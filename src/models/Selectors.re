let getSearchTerm = (model: AppCore.model) => model.search.searchTerm;

let getSearchType = (model: AppCore.model) => model.search.searchType;

let getSearchModel = (model: AppCore.model) => model.search;

let hasSearchTerm = (model: AppCore.model) =>
  getSearchTerm(model)->Js.String.length > 0;
