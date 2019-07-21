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
};

type myEpisode = {
  status,
  tags: string,
  episode,
};

type saveEpisodeData = {
  status,
  tags: string,
};

type partialEpisode = {listennotesId: string};

type myPartialEpisode = {
  status,
  episode: partialEpisode,
};

module GetMyLibraryPartial = [%graphql
  {|
  query($user_id: String!) {
    my_episodes (where: {userId: {_eq: $user_id}}) @bsRecord {
      status @bsDecoder(fn: "statusDecoder")
      episode @bsRecord{
        listennotesId
      }
    }
  }
  |}
];

module GetMyLibraryFull = [%graphql
  {|
  query($user_id: String!) {
    my_episodes (where: {userId: {_eq: $user_id}}) @bsRecord  {
      status @bsDecoder(fn: "statusDecoder")
      tags
      episode @bsRecord{
        listennotesId
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

let getPartial = () => {
  GetMyLibraryPartial.make(~user_id="margaretkru", ())
  |> Graphql.sendQuery
  |> Js.Promise.then_(result => result##my_episodes |> Js.Promise.resolve);
};

let getFull = () => {
  GetMyLibraryFull.make(~user_id="margaretkru", ())
  |> Graphql.sendQuery
  |> Js.Promise.then_(result => result##my_episodes |> Js.Promise.resolve);
};
