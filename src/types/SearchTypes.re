type podcast = {
  id: string,
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
  id: string,
  title: string,
  description: string,
  podcastTitle: string,
  pubDate: string,
  lengthSec: int,
  podcastItunesId: int,
  podcastId: string,
  genreIds: array(int),
  publisher: string,
  image: string,
};

type searchResult('a) = {
  nextOffset: int,
  total: int,
  count: int,
  results: array('a),
};

type episodeResults = searchResult(episode);
type podcastResults = searchResult(podcast);
