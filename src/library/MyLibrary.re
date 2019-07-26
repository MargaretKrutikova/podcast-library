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

type podcast = {
  listennotesId: string,
  title: string,
  description: string,
  itunesId: string,
  publisher: string,
};

type myEpisode = {
  listennotesId: string,
  title: string,
  description: string,
  pubDate: string,
  lengthSec: int,
  itunesId: option(string),
  tags: string,
  podcast,
  status,
};

let toMyEpisode = data => {
  let episode = data##episode;
  let podcast = episode##podcast;
  {
    listennotesId: episode##listennotesId,
    title: episode##title,
    description: episode##description,
    pubDate: episode##pubDate,
    lengthSec: episode##lengthSec,
    itunesId: episode##itunesId,
    tags: data##tags,
    podcast,
    status: data##status,
  };
};

type saveEpisodeData = {
  status,
  tags: string,
};

module GetMyLibrarySavedIds = [%graphql
  {|
  query($user_id: String!) {
    my_episodes (where: {userId: {_eq: $user_id}}) {
      episodeId
    }
    my_podcasts (where: {userId: {_eq: $user_id}}) {
      podcastId
    }
  }
  |}
];

type library = {
  // myEpisodes: array(myEpisode),
  myPodcasts: array(podcast),
};
type libraryIds = {
  episodes: array(string),
  podcasts: array(string),
};

let uuidToString = (id: Js.Json.t) =>
  switch (Js.Json.decodeString(id)) {
  | Some(str) => str
  | None => ""
  };

module GetMyLibrary = [%graphql
  {|
  query($user_id: String!) {
    podcasts(where: {episodes: {myEpisodes: {userId: {_eq: $user_id}}}}) @bsRecord {
      title
      listennotesId
      itunesId
      description
      publisher
    }
  }
  |}
];

module GetMyEpisodes = [%graphql
  {|
  query($user_id: String!, $podcastId: String!) {
    my_episodes(where: {episode: {podcast: {listennotesId: {_eq: $podcastId}}}, _and: {userId: {_eq: $user_id}}}) {
      episodeId
      status @bsDecoder(fn: "statusDecoder")
      tags
      episode {
        listennotesId
        itunesId
        title
        description
        pubDate
        lengthSec
      }
    }
  }
  |}
];

let getSavedIds = () => {
  GetMyLibrarySavedIds.make(~user_id="margaretkru", ())
  |> Graphql.sendQuery
  |> Js.Promise.then_(result =>
       {
         episodes:
           Belt.Array.map(result##my_episodes, myEpisode =>
             myEpisode##episodeId
           ),
         podcasts:
           Belt.Array.map(result##my_podcasts, myPodcast =>
             myPodcast##podcastId
           ),
       }
       |> Js.Promise.resolve
     );
};

let getFull = () => {
  GetMyLibrary.make(~user_id="margaretkru", ())
  |> Graphql.sendQuery
  |> Js.Promise.then_(result =>
       {
         // myEpisodes: Belt.Array.map(result##my_episodes, toMyEpisode),
         myPodcasts: result##podcasts,
       }
       |> Js.Promise.resolve
     );
};
