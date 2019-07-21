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

let statusEncoder = s =>
  switch (s) {
  | NotListened => "NotListened"
  | Started => "Started"
  | Listened => "Listened"
  };

type episode = {
  title: string,
  pubDate: string,
  podcastTitle: string,
  podcastListennotesId: string,
  listennotesId: string,
  lengthSec: int,
  description: string,
  podcastItunesId: string,
  itunesId: option(string),
};

type myEpisode = {
  episodeId: string,
  status,
  tags: string,
  episode,
};

type saveEpisodeData = {
  status,
  tags: string,
};

module GetMyLibrarySavedIds = [%graphql
  {|
  query($user_id: String!) {
    my_episodes (where: {userId: {_eq: $user_id}}) {
      episode {
        listennotesId
      }
    }
  }
  |}
];

type library = {episodes: array(myEpisode)};

let uuidToString = (id: Js.Json.t) =>
  switch (Js.Json.decodeString(id)) {
  | Some(str) => str
  | None => ""
  };

module GetMyLibrary = [%graphql
  {|
  query($user_id: String!) {
    my_episodes (where: {userId: {_eq: $user_id}}) @bsRecord  {
      episodeId
      status @bsDecoder(fn: "statusDecoder")
      tags
      episode @bsRecord{
        podcastItunesId
        listennotesId
        itunesId
        title
        description
        pubDate
        podcastTitle
        lengthSec
        podcastListennotesId
      }
    }
  }
  |}
];

let getSavedIds = () => {
  GetMyLibrarySavedIds.make(~user_id="margaretkru", ())
  |> Graphql.sendQuery
  |> Js.Promise.then_(result =>
       Belt.Array.map(result##my_episodes, myEpisode =>
         myEpisode##episode##listennotesId
       )
       |> Js.Promise.resolve
     );
};

let getFull = () => {
  GetMyLibrary.make(~user_id="margaretkru", ())
  |> Graphql.sendQuery
  |> Js.Promise.then_(result => result##my_episodes |> Js.Promise.resolve);
};
