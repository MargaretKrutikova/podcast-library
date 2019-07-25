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

let searchForEpisodes =
    (baseQuery: baseQuery, offset: int, episodeQuery: episodeQuery) => {
  let input = {
    "searchTerm": baseQuery.searchTerm,
    "language": baseQuery.language,
    "genreIds": baseQuery.genreIds,
    "offset": offset,
  };
  SearchEpisodes.make(
    ~input,
    ~episodeInput=episodeQueryToJs(episodeQuery),
    (),
  )
  |> Graphql.sendQuery
  |> Js.Promise.then_(result => result##searchEpisodes |> Js.Promise.resolve);
};
