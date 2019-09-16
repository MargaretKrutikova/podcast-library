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
};

let toMyEpisode = data => {
  let episode = data##episode;

  {
    id: episode##listennotesId,
    title: episode##title,
    description: episode##description,
    pubDate: episode##pubDate,
    lengthSec: episode##lengthSec,
    itunesId: episode##itunesId,
    tags: data##tags,
    status: data##status,
    podcastId: episode##podcastListennotesId,
    image: episode##podcast##image,
    podcastTitle: episode##podcast##title,
    publisher: episode##podcast##publisher,
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
  listennotesId: string,
  title: string,
  description: string,
  itunesId: string,
  publisher: string,
  numberOfSavedEpisodes: int,
  lastEpisodeAddedDate: option(Js.Json.t),
  podcastAddedDate: option(Js.Json.t),
};

let toMyPodcast = query => {
  listennotesId: query##listennotesId,
  title: query##title,
  description: query##description,
  itunesId: query##itunesId,
  publisher: query##publisher,
  numberOfSavedEpisodes: Utils.fromBigInt(query##numberOfEpisodes),
  lastEpisodeAddedDate: query##lastEpisodeAddedDate,
  podcastAddedDate: query##podcastAddedDate,
};

type library = {myPodcasts: array(myPodcast)};

let toMyLibrary = queryResponse => {
  myPodcasts:
    queryResponse##get_my_episodes_grouped_by_podcasts
    ->Belt.Array.map(toMyPodcast),
};
