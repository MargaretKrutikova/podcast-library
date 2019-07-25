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

let searchForPodcasts = (baseQuery: baseQuery, offset: int) => {
  let input = {
    "searchTerm": baseQuery.searchTerm,
    "language": baseQuery.language,
    "genreIds": baseQuery.genreIds,
    "offset": offset,
  };
  SearchPodcasts.make(~input, ())
  |> Graphql.sendQuery
  |> Js.Promise.then_(result => result##searchPodcasts |> Js.Promise.resolve);
};
