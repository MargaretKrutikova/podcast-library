/** my episode */
type myEpisode = {
  id: string,
  title: string,
  description: string,
  pubDate: string,
  lengthSec: int,
  itunesId: option(string),
  tags: string,
  status: EpisodeStatus.t,
  podcastId: string,
  image: string,
  podcastTitle: string,
  publisher: string,
  addedDate: option(string),
};

let toMyEpisode = data => {
  let episode = data##episode;

  {
    id: episode##id,
    title: episode##title,
    description: episode##description,
    pubDate: episode##pubDate,
    lengthSec: episode##lengthSec,
    itunesId: episode##itunesId,
    tags: data##tags,
    status: data##status,
    podcastId: episode##podcastId,
    image: episode##podcast##image,
    podcastTitle: episode##podcast##title,
    publisher: episode##podcast##publisher,
    addedDate: data##addedDate->Belt.Option.flatMap(Js.Json.decodeString),
  };
};

type episodeInsertInfo = {
  itunesId: option(string),
  podcastDescription: string,
  podcastImage: string,
};

type myEpisodeData = {
  status: EpisodeStatus.t,
  tags: string,
};

/** my podcast */
type myPodcast = {
  id: string,
  title: string,
  description: string,
  itunesId: string,
  publisher: string,
  image: string,
  addedDate: option(string),
};

let toMyPodcast = data => {
  let podcast = data##podcast;
  {
    id: data##podcastId,
    title: podcast##title,
    description: podcast##description,
    itunesId: podcast##itunesId,
    publisher: podcast##publisher,
    image: podcast##image,
    addedDate: data##addedDate |> Js.Json.decodeString,
  };
};
