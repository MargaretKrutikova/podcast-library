open SearchQuery;

type episode = {
  listennotesId: string,
  title: string,
  description: string,
  podcastTitle: string,
  pubDate: string,
  lengthSec: int,
  podcastItunesId: int,
  podcastListennotesId: string,
  genreIds: array(int),
};

type searchResult = {
  nextOffset: int,
  total: int,
  count: int,
  results: array(episode),
};

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
