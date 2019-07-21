type t = {
  id: string,
  rss: string,
  description: string,
  title: string,
  publisher: string,
  image: string,
  thumbnail: string,
  itunesId: int,
  latestPubDateMs: int,
  earliestPubDateMs: int,
  genreIds: array(int),
  totalEpisodes: int,
};

let decode = (json): t => {
  Json.Decode.{
    id: json |> field("id", string),
    rss: json |> field("rss", string),
    description: json |> field("description_original", string),
    title: json |> field("title_original", string),
    publisher: json |> field("publisher_original", string),
    image: json |> field("image", string),
    thumbnail: json |> field("thumbnail", string),
    itunesId: json |> field("itunes_id", int),
    latestPubDateMs: json |> field("latest_pub_date_ms", int),
    earliestPubDateMs: json |> field("earliest_pub_date_ms", int),
    genreIds: json |> field("genre_ids", array(int)),
    totalEpisodes: json |> field("total_episodes", int),
  };
};
