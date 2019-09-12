type t =
  | Episode
  | Podcast;

let fromString = str =>
  switch (str) {
  | s when s == "episode" => Episode
  | s when s == "podcast" => Podcast
  | _ => Podcast
  };

let toString = type_ =>
  switch (type_) {
  | Episode => "episode"
  | Podcast => "podcast"
  };
