type t = {
  id: string,
  listennotesUrl: string,
  lengthSec: int,
  rss: string,
  description: string,
  title: string,
  publisher: string,
  image: string,
  thumbnail: string,
  itunesId: int,
  pubDateMs: int,
  podcastId: string,
  genreIds: array(int),
  podcastListennotesUrl: string,
};

let decode = (json): t => {
  Json.Decode.{
    id: json |> field("id", string),
    podcastId: json |> field("podcast_id", string),
    rss: json |> field("rss", string),
    listennotesUrl: json |> field("listennotes_url", string),
    podcastListennotesUrl: json |> field("podcast_listennotes_url", string),
    lengthSec: json |> field("audio_length_sec", int),
    description: json |> field("description_original", string),
    title: json |> field("title_original", string),
    publisher: json |> field("publisher_original", string),
    image: json |> field("image", string),
    thumbnail: json |> field("thumbnail", string),
    itunesId: json |> field("itunes_id", int),
    pubDateMs: json |> field("pub_date_ms", int),
    genreIds: json |> field("genre_ids", array(int)),
  };
};
