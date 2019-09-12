open LibraryQueries;
external cast: Js.Json.t => GetMyLibrarySavedIds.t = "%identity";

module GetMyLibrarySavedIdsReadQuery =
  ApolloClient.ReadQuery(GetMyLibrarySavedIds);

module GetMyLibrarySavedIdsWriteQuery =
  ApolloClient.WriteQuery(GetMyLibrarySavedIds);

let mergeIdsCache =
    (~cache, ~myPodcasts=?, ~myEpisodes=?, ()): GetMyLibrarySavedIds.t => {
  "my_podcasts": myPodcasts->Belt.Option.getWithDefault(cache##my_podcasts),
  "my_episodes": myEpisodes->Belt.Option.getWithDefault(cache##my_episodes),
};

let updateMyLibrarySavedIds = (client, updateCache, userId) => {
  let fetchMyLibraryIds = GetMyLibrarySavedIds.make(~userId, ());
  let options = ReasonApolloHooks.Utils.toReadQueryOptions(fetchMyLibraryIds);

  switch (GetMyLibrarySavedIdsReadQuery.readQuery(client, options)) {
  | exception _ => ()
  | cachedResponse =>
    switch (cachedResponse |> Js.Nullable.toOption) {
    | None => ()
    | Some(cachedIds) =>
      let prevIds = cast(cachedIds);

      GetMyLibrarySavedIdsWriteQuery.writeQuery(
        client,
        Utils.toWriteQueryOptions(
          ~query=fetchMyLibraryIds,
          ~data=updateCache(prevIds),
        ),
      );
    }
  };
};

/** episodes */

let getSavedEpisodeId = (result: LibraryMutations.SaveEpisode.t) =>
  result##insert_episodes
  ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0))
  ->Belt.Option.flatMap(result => result##myEpisodes->Belt.Array.get(0));

let addEpisodeToCache = (~userId, client, result) => {
  switch (result |> getSavedEpisodeId) {
  | None => ()
  | Some(idObj) =>
    // update cache with ids
    let updateCache = cache => {
      let myEpisodes = cache##my_episodes->Belt.Array.concat([|idObj|]);
      mergeIdsCache(~cache, ~myEpisodes, ());
    };

    updateMyLibrarySavedIds(client, updateCache, userId);
  };
};

let getRemovedEpisodeId = (result: LibraryMutations.RemoveEpisode.t) =>
  result##delete_my_episodes
  ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0));

let removeEpisodeFromCache = (~userId, client, result) => {
  switch (result |> getRemovedEpisodeId) {
  | None => ()
  | Some(idObj) =>
    let updateCache = cache => {
      let myEpisodes =
        cache##my_episodes
        ->Belt.Array.keep(obj => obj##episodeId !== idObj##episodeId);
      mergeIdsCache(~cache, ~myEpisodes, ());
    };

    updateMyLibrarySavedIds(client, updateCache, userId);
  };
};

/** podcasts */
let getRemovedPodcastId = (result: LibraryMutations.RemovePodcast.t) =>
  result##delete_my_podcasts
  ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0));

let removePodcastFromCache = (~userId, client, result) => {
  switch (result |> getRemovedPodcastId) {
  | None => ()
  | Some(idObj) =>
    let updateIdsCache = cache => {
      let myPodcasts =
        cache##my_podcasts
        ->Belt.Array.keep(obj => obj##podcastId !== idObj##podcastId);
      mergeIdsCache(~cache, ~myPodcasts, ());
    };

    updateMyLibrarySavedIds(client, updateIdsCache, userId);
  };
};

let getSavedPodcastId = (result: LibraryMutations.SavePodcast.t) =>
  result##insert_my_podcasts
  ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0));

let addPodcastToCache = (~userId, client, result) => {
  switch (result |> getSavedPodcastId) {
  | None => ()
  | Some(idObj) =>
    let updateCache = cache => {
      let myPodcasts = cache##my_podcasts->Belt.Array.concat([|idObj|]);
      mergeIdsCache(~cache, ~myPodcasts, ());
    };

    updateMyLibrarySavedIds(client, updateCache, userId);
  };
};
