type episodeQuery = {
  // minAudioLength: option(int),
  // maxAudioLength: option(int),
  podcastId: option(string),
  excludePodcastId: option(string),
};

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
type searchInput = {
  searchTerm: string,
  language: option(string),
  genreIds: array(int),
  offset: int,
  // sort,
  // searchFields: option(array(searchField)),
  // publishedAfterDate: option(Js.Date.t),
  // publishedBeforeDate: option(Js.Date.t),
};

let dateToMsString = date => Js.Float.toString @@ Js.Date.getTime(date);
