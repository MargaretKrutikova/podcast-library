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

type message =
  | EnteredSearchTerm(string)
  | SetContentType(ContentType.t);

/** update */

let updateBaseSearchQuery = (model, baseQuery) => {...model, baseQuery};

let updateEpisodeSearchQuery = (model, episodeQuery) => {
  ...model,
  episodeQuery,
};

let update = (model, message) =>
  switch (message) {
  | EnteredSearchTerm(searchTerm) => (
      updateBaseSearchQuery(model, {...model.baseQuery, searchTerm}),
      None,
    )
  | SetContentType(searchType) => ({...model, searchType}, None)
  };
