type t = {
  id: string,
  rss: string,
  description: string,
  title: string,
  publisher: string,
  image: string,
  thumbnail: string,
  itunes_id: int,
  latest_pub_date_ms: int,
  earliest_pub_date_ms: int,
  genre_ids: array(int),
  total_episodes: int,
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
    itunes_id: json |> field("itunes_id", int),
    latest_pub_date_ms: json |> field("latest_pub_date_ms", int),
    earliest_pub_date_ms: json |> field("earliest_pub_date_ms", int),
    genre_ids: json |> field("genre_ids", array(int)),
    total_episodes: json |> field("total_episodes", int),
  };
};
