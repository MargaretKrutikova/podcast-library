let setWindowLocation: string => unit = [%bs.raw
  {| function (href) { window.location.href = href; } |}
];

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

let maxDescrptionTextLength = 200;

let truncateDescription = description =>
  String.length(description) < maxDescrptionTextLength
    ? description : description ++ "...";

/** reason apollo */
let convertToQueryObj = (result): ReasonApolloTypes.queryObj => {
  "query": ApolloClient.gql(. result##query),
  "variables": result##variables,
};

let hasMutationData = (result: ReasonApolloTypes.mutationResponse('a)) => {
  switch (result) {
  | Data(_) => true
  | _ => false
  };
};
