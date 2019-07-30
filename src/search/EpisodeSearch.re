open SearchQuery;
open SearchResult;

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

let makeSearchQuery =
    (baseQuery: baseQuery, offset: int, episodeQuery: episodeQuery) => {
  SearchEpisodes.make(
    ~input=toSearchInput(~baseQuery, ~offset),
    ~episodeInput=episodeQueryToJs(episodeQuery),
    (),
  );
};

let fetchMoreUpdateQuery: ReasonApolloQuery.updateQueryT =
  SearchResult.fetchMoreUpdateQuery("searchEpisodes");
