let setLocation: string => unit = [%bs.raw
  {| function (href) { window.location.href = href; } |}
];

let baseUrl = "https://podcasts.apple.com/us/podcast/";

let makeForPodcast = podcastItunesId => baseUrl ++ "id" ++ podcastItunesId;

let makeForEpisode = (~podcastItunesId, ~episodeItunesId) =>
  baseUrl
  ++ episodeItunesId
  ++ "/id"
  ++ podcastItunesId
  ++ "?i="
  ++ episodeItunesId;
