type resultType =
  | Podcast(PodcastResult.t)
  | Episode(EpisodeResult.t);

type t = {
  count: int,
  next_offset: int,
  total: int,
  results: array(resultType),
};

Json.Decode.either;
let decode = (json): t => {
  Json.Decode.{
    count: json |> field("count", int),
    next_offset: json |> field("next_offset", int),
    total: json |> field("total", int),
    results:
      json
      |> field(
           "results",
           array(
             either(
               PodcastResult.decode |> map(r => Podcast(r)),
               EpisodeResult.decode |> map(r => Episode(r)),
             ),
           ),
         ),
  };
};
