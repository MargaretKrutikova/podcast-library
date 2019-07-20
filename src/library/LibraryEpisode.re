type status =
  | NotListened
  | Started
  | Listened;

let statusDecoder = s =>
  switch (s) {
  | "NotListened" => NotListened
  | "Started" => Started
  | "Listened" => Listened
  | _ => NotListened
  };

type episode = {
  id: string,
  title: string,
  pub_date_ms: int,
  podcast_title: string,
  podcast_listennotes_id: string,
  listennotes_id: string,
  length_sec: int,
  itunes_id: string,
  description: option(string),
  genre_ids: option(string),
};

type myEpisode = {
  status,
  tags: string,
  episode: option(episode),
};

type saveEpisodeData = {
  status,
  tags: string,
};

type partialEpisode = {listennotes_id: string};

type myPartialEpisode = {
  status,
  episode: partialEpisode,
};
