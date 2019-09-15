open SearchQuery;
open SearchTypes;

let toSearchTypeVariant = (searchType: ContentType.t) =>
  switch (searchType) {
  | Episode => `EPISODE
  | Podcast => `PODCAST
  };

let toSearchInput = (~query: SearchQuery.t, ~offset) => {
  "searchTerm": query.searchTerm,
  "language": query.language,
  "genreIds": query.genreIds,
  "offset": offset,
  "podcastId": query.podcastId,
  "searchType": toSearchTypeVariant(query.searchType),
  "excludePodcastId": None,
};

module Search = [%graphql
  {|
  query($input: SearchInput!) {
    search (input: $input) @bsRecord  {
      nextOffset
      total
      count
      results {
        ... on Podcast @bsRecord {
          id
          description
          title
          publisher
          podcastItunesId
          latestPubDate
          genreIds
          totalEpisodes
          image
        }
        ... on Episode @bsRecord {
          id
          podcastTitle
          title
          podcastItunesId
          lengthSec
          podcastId
          description
          pubDate
          genreIds
          publisher
        }
      }
    }
  }
  |}
];

let makeSearchQuery = (query: SearchQuery.t, offset: int) => {
  Search.make(~input=toSearchInput(~query, ~offset), ());
};

/** fetch more */
let fetchMoreUpdateQuery =
    (
      prevResult: Js.Json.t,
      options: ReasonApolloHooks.Query.updateQueryOptions,
    ) => {
  let mergeResults: (Js.Json.t, option(Js.Json.t)) => Js.Json.t = [%bs.raw
    {|
    function (prevResult, fetchMoreResult) {
      if (!fetchMoreResult) return prevResult;

      const results = prevResult.search.results.concat(
        fetchMoreResult.search.results
      );

      return {
        ...fetchMoreResult,
        search: { ...fetchMoreResult.search, results }
      };
    }
   |}
  ];
  mergeResults(
    prevResult,
    options->ReasonApolloHooks.Query.fetchMoreResultGet,
  );
};
