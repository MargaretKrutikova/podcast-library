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

let searchForEpisodes = (baseQuery: baseQuery, episodeQuery: episodeQuery) => {
  SearchEpisodes.make(
    ~input=baseQueryToJs(baseQuery),
    ~episodeInput=episodeQueryToJs(episodeQuery),
    (),
  )
  |> Graphql.sendQuery
  |> Js.Promise.then_(result => result##searchEpisodes |> Js.Promise.resolve);
};
