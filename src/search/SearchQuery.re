[@bs.deriving jsConverter]
type episodeQuery = {
  podcastId: option(string),
  excludePodcastId: option(string),
};

let createEpisodeQuery = () => {podcastId: None, excludePodcastId: None};

type sort =
  | Relevance
  | Date;

let sortToParam = sort => {
  string_of_int(
    switch (sort) {
    | Relevance => 0
    | Date => 1
    },
  );
};

type searchField =
  | Title
  | Description
  | Author
  | Audio;

let searchFieldToParam = field => {
  switch (field) {
  | Title => "title"
  | Description => "description"
  | Author => "author"
  | Audio => "audio"
  };
};

[@bs.deriving jsConverter]
type baseQuery = {
  searchTerm: string,
  language: option(string),
  genreIds: option(array(int)),
  offset: int,
  // sort,
  // searchFields: option(array(searchField)),
  // publishedAfterDate: option(Js.Date.t),
  // publishedBeforeDate: option(Js.Date.t),
};

let createBaseQuery = () => {
  searchTerm: "",
  language: None,
  genreIds: None,
  offset: 0,
};
