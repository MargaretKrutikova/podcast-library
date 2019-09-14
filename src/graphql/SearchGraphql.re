open SearchQuery;
open SearchTypes;

let toSearchInput = (~baseQuery: SearchQuery.BaseQuery.t, ~offset) => {
  "searchTerm": baseQuery.searchTerm,
  "language": baseQuery.language,
  "genreIds": baseQuery.genreIds,
  "offset": offset,
};

module SearchPodcasts = [%graphql
  {|
  query($input: BaseSearchInput!) {
    searchPodcasts (input: $input) @bsRecord  {
      nextOffset
      total
      count
      results @bsRecord {
        listennotesId
        description
        title
        publisher
        podcastItunesId
        latestPubDate
        genreIds
        totalEpisodes
        image
      }
    }
  }
  |}
];

let makeSearchPodcastsQuery =
    (baseQuery: SearchQuery.BaseQuery.t, offset: int) =>
  SearchPodcasts.make(~input=toSearchInput(~baseQuery, ~offset), ());

module SearchEpisodes = [%graphql
  {|
  query($input: BaseSearchInput!, $episodeInput: EpisodeSearchInput!) {
    searchEpisodes (input: $input, episodeInput: $episodeInput) @bsRecord  {
      nextOffset
      total
      count
      results @bsRecord {
        listennotesId
        podcastTitle
        title
        podcastItunesId
        lengthSec
        podcastListennotesId
        description
        pubDate
        genreIds
        publisher
      }
    }
  }
  |}
];

let makeSearchEpisodesQuery =
    (
      baseQuery: SearchQuery.BaseQuery.t,
      offset: int,
      episodeQuery: SearchQuery.EpisodeQuery.t,
    ) => {
  SearchEpisodes.make(
    ~input=toSearchInput(~baseQuery, ~offset),
    ~episodeInput=SearchQuery.EpisodeQuery.tToJs(episodeQuery),
    (),
  );
};

/** fetch more */
let fetchMoreUpdateQuery =
    (
      queryName,
      prevResult: Js.Json.t,
      options: ReasonApolloHooks.Query.updateQueryOptions,
    ) => {
  let mergeResults: (string, Js.Json.t, option(Js.Json.t)) => Js.Json.t = [%bs.raw
    {|
    function (queryName, prevResult, fetchMoreResult) {
      if (!fetchMoreResult) return prevResult;

      const results = prevResult[queryName].results.concat(
        fetchMoreResult[queryName].results
      );

      return {
        ...fetchMoreResult,
        [queryName]: { ...fetchMoreResult[queryName], results }
      };
    }
   |}
  ];
  mergeResults(
    queryName,
    prevResult,
    options->ReasonApolloHooks.Query.fetchMoreResultGet,
  );
};

let fetchMoreEpisodes = fetchMoreUpdateQuery("searchEpisodes");
let fetchMorePodcasts = fetchMoreUpdateQuery("searchPodcasts");
