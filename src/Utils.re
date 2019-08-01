let setWindowLocation: string => unit = [%bs.raw
  {| function (href) { window.location.href = href; } |}
];

/** itunes */
let itunesBaseUrl = "https://podcasts.apple.com/us/podcast/";

let makePodcastItunesUrl = podcastItunesId =>
  itunesBaseUrl ++ "id" ++ podcastItunesId;

let makeEpisodeItunesUrl = (~podcastItunesId, ~episodeItunesId) =>
  itunesBaseUrl
  ++ episodeItunesId
  ++ "/id"
  ++ podcastItunesId
  ++ "?i="
  ++ episodeItunesId;

/** UI */
let maxDescrptionTextLength = 200;

let truncateDescription = description =>
  String.length(description) < maxDescrptionTextLength
    ? description : description ++ "...";

/** reason apollo */
let toQueryObj = (result): ReasonApolloTypes.queryObj => {
  "query": ApolloClient.gql(. result##query),
  "variables": result##variables,
};

let toReadQueryOptions = result => {
  "query": ApolloClient.gql(. result##query),
  "variables": Js.Nullable.fromOption(Some(result##variables)),
};

let hasMutationData = (result: ReasonApolloTypes.mutationResponse('a)) => {
  switch (result) {
  | Data(_) => true
  | _ => false
  };
};

/** api data */
let toApiGenres = (arr: array(int)) => Js.Array.joinWith(",", arr);
