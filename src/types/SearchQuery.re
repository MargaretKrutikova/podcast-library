module BaseQuery = {
  [@bs.deriving jsConverter]
  type t = {
    searchTerm: string,
    language: option(string),
    genreIds: option(array(int)),
  };

  let make = (~searchTerm="", ()) => {
    searchTerm,
    language: Some("English"),
    genreIds: None,
  };
};

module EpisodeQuery = {
  [@bs.deriving jsConverter]
  type t = {
    podcastId: option(string),
    excludePodcastId: option(string),
  };

  let make = () => {podcastId: None, excludePodcastId: None};
};

type t = {
  searchType: ContentType.t,
  baseQuery: BaseQuery.t,
  episodeQuery: EpisodeQuery.t,
};

let make = (~searchType, ~searchTerm="", ()) => {
  searchType,
  baseQuery: BaseQuery.make(~searchTerm, ()),
  episodeQuery: EpisodeQuery.make(),
};

module Url = {
  type t = {
    searchTerm: string,
    content: ContentType.t,
  };

  let valueFromDict = (key, dict) => {
    switch (Js.Dict.get(dict, key)) {
    | Some(Url.Query.Single(search)) => search
    | Some(Multiple(array)) when Js.Array.length(array) > 0 =>
      Js.Array.unsafe_get(array, 0)
    | _ => ""
    };
  };

  let parse = queryString => {
    let dict = Url.Query.parse(queryString);
    let searchTerm = valueFromDict("q", dict);
    let content = valueFromDict("content", dict) |> ContentType.fromString;

    {searchTerm, content};
  };

  let stringify = query => {
    [||]
    |> Url.Params.add("q", query.searchTerm)
    |> Url.Params.add("content", query.content |> ContentType.toString);
  };

  let fromSearchQuery = (query): t => {
    searchTerm: query.baseQuery.searchTerm,
    content: query.searchType,
  };

  let toSearchQuery = (params: t) =>
    make(~searchType=params.content, ~searchTerm=params.searchTerm, ());
};

/** not used yet */

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
