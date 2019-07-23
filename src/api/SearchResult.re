type podcast = {
  listennotesId: string,
  description: string,
  title: string,
  publisher: string,
  podcastItunesId: int,
  latestPubDate: string,
  genreIds: array(int),
  totalEpisodes: int,
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
