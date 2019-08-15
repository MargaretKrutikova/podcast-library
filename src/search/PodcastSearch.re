open SearchQuery;
open SearchResult;

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

let makeSearchQuery = (baseQuery: baseQuery, offset: int) =>
  SearchPodcasts.make(~input=toSearchInput(~baseQuery, ~offset), ());

let fetchMoreUpdateQuery: ReasonApolloHooks.Query.updateQueryT =
  SearchResult.fetchMoreUpdateQuery("searchPodcasts");
