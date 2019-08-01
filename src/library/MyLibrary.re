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

external toSavedIds: Js.Json.t => GetMyLibrarySavedIds.t = "%identity";

// this is necessary to keep __typename apollo field on the root cache query
let mergeCacheJs:
  (GetMyLibrarySavedIds.t, GetMyLibrarySavedIds.t) => GetMyLibrarySavedIds.t = [%bs.raw
  {|
      function (prev, next) {
        return { ...prev, ...next };
      }
    |}
];

let mergeCache =
    (~cache: GetMyLibrarySavedIds.t, ~myPodcasts=?, ~myEpisodes=?, ())
    : GetMyLibrarySavedIds.t => {
  "my_podcasts": myPodcasts->Belt.Option.getWithDefault(cache##my_podcasts),
  "my_episodes": myEpisodes->Belt.Option.getWithDefault(cache##my_episodes),
};

let updateMyLibrarySavedIds =
    (client, updateCache: GetMyLibrarySavedIds.t => GetMyLibrarySavedIds.t) => {
  let fetchMyLibraryIds = makeGetSavedIdsQuery();
  let cachedResponse =
    GetMyLibrarySavedIdsReadQuery.readQuery(
      client,
      Utils.toReadQueryOptions(fetchMyLibraryIds),
    );

  switch (cachedResponse |> Js.Nullable.toOption) {
  | None => ()
  | Some(cachedIds) =>
    let savedIds = toSavedIds(cachedIds);

    let updatedCachedIds = updateCache(savedIds);
    GetMyLibrarySavedIdsWriteQuery.make(
      ~client,
      ~variables=fetchMyLibraryIds##variables,
      ~data=updatedCachedIds |> mergeCacheJs(savedIds),
      (),
    );
  };
};

let removePodcastIdFromCache = (client, mutationResult) => {
  let removedId = RemoveContent.getRemovedPodcastId(mutationResult);

  switch (removedId) {
  | None => ()
  | Some(idObj) =>
    let updateCache = cache => {
      let myPodcasts =
        cache##my_podcasts
        ->Belt.Array.keep(obj => obj##podcastId !== idObj##podcastId);
      mergeCache(~cache, ~myPodcasts, ());
    };
    updateMyLibrarySavedIds(client, updateCache);
  };
};

let removeEpisodeIdFromCache = (client, mutationResult) => {
  let removedId = RemoveContent.getRemovedEpisodeId(mutationResult);

  switch (removedId) {
  | None => ()
  | Some(idObj) =>
    let updateCache = cache => {
      let myEpisodes =
        cache##my_episodes
        ->Belt.Array.keep(obj => obj##episodeId !== idObj##episodeId);
      mergeCache(~cache, ~myEpisodes, ());
    };

    updateMyLibrarySavedIds(client, updateCache);
  };
};

let addEpisodeIdToCache = (client, mutationResult) => {
  let insertedId = SaveEpisode.getSavedId(mutationResult);

  switch (insertedId) {
  | None => ()
  | Some(idObj) =>
    let updateCache = cache => {
      let myEpisodes = cache##my_episodes->Belt.Array.concat([|idObj|]);
      mergeCache(~cache, ~myEpisodes, ());
    };

    updateMyLibrarySavedIds(client, updateCache);
  };
  ();
};

let addPodcastIdToCache = (client, mutationResult) => {
  let insertedId = SavePodcast.getSavedId(mutationResult);

  switch (insertedId) {
  | None => ()
  | Some(idObj) =>
    let updateCache = cache => {
      let myPodcasts = cache##my_podcasts->Belt.Array.concat([|idObj|]);
      mergeCache(~cache, ~myPodcasts, ());
    };

    updateMyLibrarySavedIds(client, updateCache);
  };
  ();
};
