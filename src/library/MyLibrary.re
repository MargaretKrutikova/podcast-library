/** my episodes */
module GetMyEpisodes = [%graphql
  {|
query($userId: String!, $podcastId: String!) {
  my_episodes(where: {episode: {podcast: {listennotesId: {_eq: $podcastId}}}, _and: {userId: {_eq: $userId}}}) {
    episodeId
    status @bsDecoder(fn: "EpisodeStatus.decode")
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
  podcasts(limit: 1, where: {listennotesId: {_eq: $podcastId}}) {
      title
      description
      itunesId
    }
}
|}
];

type myEpisode = {
  listennotesId: string,
  title: string,
  description: string,
  pubDate: string,
  lengthSec: int,
  itunesId: option(string),
  tags: string,
  status: EpisodeStatus.t,
};

let toMyEpisode = data => {
  let episode = data##episode;
  {
    listennotesId: episode##listennotesId,
    title: episode##title,
    description: episode##description,
    pubDate: episode##pubDate,
    lengthSec: episode##lengthSec,
    itunesId: episode##itunesId,
    tags: data##tags,
    status: data##status,
  };
};

let makeGetMyEpisodesQuery = (~podcastId) =>
  GetMyEpisodes.make(~userId="margaretkru", ~podcastId, ());

/** my library */

type myPodcast = {
  listennotesId: string,
  title: string,
  description: string,
  itunesId: string,
  publisher: string,
  numberOfSavedEpisodes: int,
  lastEpisodeAddedDate: option(Js.Json.t),
  podcastAddedDate: option(Js.Json.t),
};

type library = {myPodcasts: array(myPodcast)};
let fromBigInt = value =>
  value->Js.Json.decodeNumber->Belt.Option.mapWithDefault(0, int_of_float);

let makePositive = number => number < 0 ? 0 : number;

let updatePodcastEpisodeCount = (podcast: 'a, updateCount: int => int): 'a => {
  let merge: ('a, int) => 'a = [%raw
    {|
    function(prev, numberOfEpisodes) {
      return {...prev, numberOfEpisodes}
    }
  |}
  ];

  let numberOfEpisodes =
    podcast##numberOfEpisodes |> fromBigInt |> updateCount |> makePositive;

  merge(podcast, numberOfEpisodes);
};

module GetMyLibrary = [%graphql
  {|
  query($userId: String!) {
    get_my_episodes_grouped_by_podcasts (args:{userid: $userId}) {
        title
        numberOfEpisodes
        lastEpisodeAddedDate
        podcastAddedDate
        listennotesId
        description
        itunesId
        publisher
      }
    }
  |}
];

let toMyPodcast = query => {
  listennotesId: query##listennotesId,
  title: query##title,
  description: query##description,
  itunesId: query##itunesId,
  publisher: query##publisher,
  numberOfSavedEpisodes: fromBigInt(query##numberOfEpisodes),
  lastEpisodeAddedDate: query##lastEpisodeAddedDate,
  podcastAddedDate: query##podcastAddedDate,
};

let toMyLibrary = queryResponse => {
  myPodcasts:
    queryResponse##get_my_episodes_grouped_by_podcasts
    ->Belt.Array.map(toMyPodcast),
};

let makeGetMyLibraryQuery = (~userId) => GetMyLibrary.make(~userId, ());

/** saved ids */
module GetMyLibrarySavedIds = [%graphql
  {|
  query($userId: String!) {
    my_episodes (where: {userId: {_eq: $userId}}) {
      episodeId
    }
    my_podcasts (where: {userId: {_eq: $userId}}) {
      podcastId
    }
  }
  |}
];

let makeGetSavedIdsQuery = (~userId) =>
  GetMyLibrarySavedIds.make(~userId, ());
