let baseUrl = "https://podcasts.apple.com/us/podcast";

let makeForPodcast = podcastItunesId => baseUrl ++ "/id" ++ podcastItunesId;

let makeForEpisode = (~podcastItunesId, ~episodeItunesId) =>
  baseUrl
  ++ episodeItunesId
  ++ "/id"
  ++ podcastItunesId
  ++ "?i="
  ++ episodeItunesId;
