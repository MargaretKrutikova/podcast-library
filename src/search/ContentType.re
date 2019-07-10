type t =
  | Episode
  | Podcast;

let episodeApiType = "episode";
let podcastApiType = "podcast";

let fromApiType = str =>
  switch (str) {
  | s when s == episodeApiType => Episode
  | s when s == podcastApiType => Podcast
  | _ => Podcast
  };

let decode = json => json |> Json.Decode.string |> fromApiType;
