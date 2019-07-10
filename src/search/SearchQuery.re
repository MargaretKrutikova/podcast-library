module EpisodeQuery = {
  open UrlBuilder.Params;

  type t = {
    minAudioLength: option(int),
    maxAudioLength: option(int),
    podcastId: option(string),
    excludePodcastId: option(string),
  };

  let toParams = query => {
    [||]
    |> addOption(
         "len_min",
         Belt.Option.map(query.minAudioLength, string_of_int),
       )
    |> addOption(
         "len_max",
         Belt.Option.map(query.maxAudioLength, string_of_int),
       )
    |> addOption("ocid", query.podcastId)
    |> addOption("ncid", query.excludePodcastId);
  };

  let defaultValue = (): t => {
    minAudioLength: None,
    maxAudioLength: None,
    podcastId: None,
    excludePodcastId: None,
  };
};

type contentQuery =
  | PodcastQuery
  | EpisodeQuery(EpisodeQuery.t);

let contentToParams = contentQuery => {
  switch (contentQuery) {
  | PodcastQuery => [|("type", "podcast")|]
  | EpisodeQuery(query) =>
    EpisodeQuery.toParams(query) |> UrlBuilder.Params.add("type", "episode")
  };
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

let join = values => values |> Js.Array.joinWith(",");

let searchFieldsToParam = fields =>
  fields->Belt.Option.map(values =>
    values->Belt.Array.map(searchFieldToParam) |> join
  );

type t = {
  sort,
  searchTerm: string,
  language: string,
  searchFields: option(array(searchField)),
  publishedAfterDate: option(Js.Date.t),
  publishedBeforeDate: option(Js.Date.t),
  genreIds: option(array(int)),
  offset: int,
  content: contentQuery,
};

let dateToMsString = date => Js.Float.toString @@ Js.Date.getTime(date);

let toParams = query => {
  UrlBuilder.Params.(
    [||]
    |> add("safe_mode", "1")  // Exclude content with explicit language
    |> add("q", query.searchTerm)
    |> add("sort_by_date", sortToParam(query.sort))
    |> add("language", query.language)
    |> add("offset", string_of_int(query.offset))
    |> addOption("only_in", searchFieldsToParam(query.searchFields))
    |> addOption(
         "published_after",
         query.publishedAfterDate->Belt.Option.map(dateToMsString),
       )
    |> addOption(
         "published_before",
         query.publishedBeforeDate->Belt.Option.map(dateToMsString),
       )
    |> addOption(
         "genre_ids",
         query.genreIds
         ->Belt.Option.map(ids => ids->Belt.Array.map(string_of_int) |> join),
       )
    |> Belt.Array.concat(contentToParams(query.content))
  );
};

let create =
    (
      ~searchTerm,
      ~sort=Relevance,
      ~language="English", // TODO: fetch languages
      ~searchFields=None,
      ~genreIds=None,
      ~offset=0,
      ~content,
      (),
    ) => {
  searchTerm,
  sort,
  language,
  searchFields,
  publishedAfterDate: None,
  publishedBeforeDate: None,
  genreIds,
  offset,
  content,
};
