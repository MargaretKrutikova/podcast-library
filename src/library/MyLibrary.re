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
  title: string,
  description: string,
  publisher: string,
  itunesId: string,
  genreIds: string,
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
    my_episodes (where: {userId: {_eq: $user_id}})  {
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
        podcast @bsRecord {
          title
          description
          itunesId
          publisher
          genreIds
        }
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
  |> Js.Promise.then_(result =>
       Belt.Array.map(result##my_episodes, toMyEpisode) |> Js.Promise.resolve
     );
};
