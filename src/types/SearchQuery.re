[@bs.deriving jsConverter]
type t = {
  searchTerm: string,
  searchType: ContentType.t,
  language: option(string),
  podcastId: option(string),
  genreIds: option(array(int)),
};

let make = (~searchTerm="", ()) => {
  searchTerm,
  language: Some("English"),
  searchType: Episode,
  genreIds: None,
  podcastId: None,
};

module Url = {
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
    let podcastId = valueFromDict("podcastid", dict);
    let searchType = valueFromDict("content", dict) |> ContentType.fromString;

    {
      searchTerm,
      searchType,
      language: Some("English"),
      genreIds: None,
      podcastId: Some(podcastId),
    };
  };

  let stringify = query => {
    [||]
    |> Url.Params.add("q", query.searchTerm)
    |> Url.Params.addOption("podcastid", query.podcastId)
    |> Url.Params.add("content", query.searchType |> ContentType.toString);
  };
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
