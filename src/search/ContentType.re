type t =
  | Episode
  | Podcast
  | Unknown;

let episodeApiType = "episode";
let podcastApiType = "podcast";

let fromApiType = str =>
  switch (str) {
  | s when s == episodeApiType => Episode
  | s when s == podcastApiType => Podcast
  | _ => Unknown
  };

let decode = json => json |> Json.Decode.string |> fromApiType;
