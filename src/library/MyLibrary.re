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

/** my episodes */
module GetMyEpisodes = [%graphql
  {|
query($userId: String!, $podcastId: String!) {
  my_episodes(where: {episode: {podcast: {listennotesId: {_eq: $podcastId}}}, _and: {userId: {_eq: $userId}}}) {
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
  // podcast,
  status,
};

let toMyEpisode = data => {
  let episode = data##episode;
  //let podcast = episode##podcast;
  {
    listennotesId: episode##listennotesId,
    title: episode##title,
    description: episode##description,
    pubDate: episode##pubDate,
    lengthSec: episode##lengthSec,
    itunesId: episode##itunesId,
    tags: data##tags,
    // podcast,
    status: data##status,
  };
};

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

module GetMyLibrary = [%graphql
  {|
  query($user_id: String!) {
    get_my_episodes_grouped_by_podcasts (args:{userid: $user_id}) {
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

let getMyLibraryQuery = () => GetMyLibrary.make(~user_id="margaretkru", ());

type saveEpisodeData = {
  status,
  tags: string,
};

/** saved ids */
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

let isEpisodeSaved =
    (
      response: ReasonApolloTypes.queryResponse(GetMyLibrarySavedIds.t),
      episodeId,
    ) =>
  switch (response) {
  | Data(savedIds) =>
    savedIds##my_episodes
    ->Belt.Array.some(idObj => idObj##episodeId === episodeId)
  | _ => false
  };

let isPodcastSaved =
    (
      response: ReasonApolloTypes.queryResponse(GetMyLibrarySavedIds.t),
      podcastId,
    ) =>
  switch (response) {
  | Data(savedIds) =>
    savedIds##my_podcasts
    ->Belt.Array.some(idObj => idObj##podcastId === podcastId)
  | _ => false
  };

let makeGetSavedIdsQuery = () =>
  GetMyLibrarySavedIds.make(~user_id="margaretkru", ());

module GetMyLibrarySavedIdsReadQuery =
  ApolloClient.ReadQuery(GetMyLibrarySavedIds);

module GetMyLibrarySavedIdsWriteQuery =
  ApolloClient.WriteQuery(GetMyLibrarySavedIds);

let addEpisodeIdToSaved:
  ({. "episodeId": string}, Js.Json.t) => GetMyLibrarySavedIds.t = [%bs.raw
  {|
      function (episodeId, cache) {
        return {
          ...cache,
          my_episodes: [...cache.my_episodes, episodeId]
        };
      }
    |}
];

let addPodcastIdToSaved:
  ({. "podcastId": string}, Js.Json.t) => GetMyLibrarySavedIds.t = [%bs.raw
  {|
      function (podcastId, cache) {
        return {
          ...cache,
          my_podcasts: [...cache.my_podcasts, podcastId]
        };
      }
    |}
];

let updateMyLibrarySavedIds =
    (client, updateCache: Js.Json.t => GetMyLibrarySavedIds.t) => {
  let fetchMyLibraryIds = makeGetSavedIdsQuery();
  let cachedResponse =
    GetMyLibrarySavedIdsReadQuery.readQuery(
      client,
      Utils.toReadQueryOptions(fetchMyLibraryIds),
    );

  switch (cachedResponse |> Js.Nullable.toOption) {
  | None => ()
  | Some(cachedIds) =>
    let updatedCachedIds = updateCache(cachedIds);
    GetMyLibrarySavedIdsWriteQuery.make(
      ~client,
      ~variables=fetchMyLibraryIds##variables,
      ~data=updatedCachedIds,
      (),
    );
  };
};

let addEpisodeIdToCache = (client, mutationResult) => {
  let insertedEpisode =
    mutationResult##data
    ->Belt.Option.flatMap(result => result##insert_episodes)
    ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0))
    ->Belt.Option.flatMap(result => result##myEpisodes->Belt.Array.get(0));

  switch (insertedEpisode) {
  | None => ()
  | Some(episode) =>
    updateMyLibrarySavedIds(client, addEpisodeIdToSaved(episode))
  };
  ();
};

let addPodcastIdToCache = (client, mutationResult) => {
  let insertedPodcast =
    mutationResult##data
    ->Belt.Option.flatMap(result => result##insert_my_podcasts)
    ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0));

  switch (insertedPodcast) {
  | None => ()
  | Some(podcast) =>
    updateMyLibrarySavedIds(client, addPodcastIdToSaved(podcast))
  };
  ();
};
