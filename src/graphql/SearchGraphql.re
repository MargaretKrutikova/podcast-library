open SearchQuery;
open SearchTypes;

let toSearchInput = (~query: SearchQuery.t, ~offset) => {
  "searchTerm": query.searchTerm,
  "language": query.language,
  "genreIds": query.genreIds,
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

let makeSearchPodcastsQuery = (query: SearchQuery.t, offset: int) =>
  SearchPodcasts.make(~input=toSearchInput(~query, ~offset), ());

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
        image
      }
    }
  }
  |}
];

let makeSearchEpisodesQuery = (query: SearchQuery.t, offset: int) => {
  SearchEpisodes.make(
    ~input=toSearchInput(~query, ~offset),
    ~episodeInput={"podcastId": query.podcastId, "excludePodcastId": None},
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
