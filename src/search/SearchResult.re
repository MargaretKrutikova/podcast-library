type podcast = {
  listennotesId: string,
  description: string,
  title: string,
  publisher: string,
  podcastItunesId: int,
  latestPubDate: string,
  genreIds: array(int),
  totalEpisodes: int,
  image: string,
};

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
  publisher: string,
};

type searchResult('a) = {
  nextOffset: int,
  total: int,
  count: int,
  results: array('a),
};

type episodeResults = searchResult(episode);
type podcastResults = searchResult(podcast);

let fetchMoreUpdateQuery =
    (
      queryName,
      prevResult: Js.Json.t,
      options: ReasonApolloQuery.updateQueryOptions,
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
    options->ReasonApolloQuery.fetchMoreResultGet,
  );
};

let toSearchInput = (~baseQuery: SearchQuery.baseQuery, ~offset) => {
  "searchTerm": baseQuery.searchTerm,
  "language": baseQuery.language,
  "genreIds": baseQuery.genreIds,
  "offset": offset,
};
