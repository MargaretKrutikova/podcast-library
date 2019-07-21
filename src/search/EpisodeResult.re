open SearchQuery;

type episode = {
  listennotesId: string,
  title: string,
  description: string,
  podcastTitle: string,
  pubDateMs: int,
  lengthSec: int,
  podcastItunesId: int,
  podcastListennotesId: string,
};

type searchResult = {
  nextOffset: int,
  total: int,
  count: int,
  results: array(episode),
};

module SearchEpisodes = [%graphql
  {|
  query($searchTerm: String!, $offset: Int!) {
    searchEpisodes (input: {searchTerm: $searchTerm, offset: $offset}) @bsRecord  {
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
        pubDateMs
      }
    }
  }
  |}
];

let searchForEpisodes = (input: searchInput) => {
  //, episodeInput: option(SearchQuery.episodeQuery)) => {
  SearchEpisodes.make(~searchTerm=input.searchTerm, ~offset=input.offset, ())
  |> Graphql.sendQuery;
};
