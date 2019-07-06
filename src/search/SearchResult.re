type t = {
  count: int,
  next_offset: int,
  total: int,
  results: array(PodcastResult.t),
};

let decode = (json): t => {
  Json.Decode.{
    count: json |> field("count", int),
    next_offset: json |> field("next_offset", int),
    total: json |> field("total", int),
    results: json |> field("results", array(PodcastResult.decode)),
  };
};
