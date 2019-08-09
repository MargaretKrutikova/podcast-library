/** episode */
module RemoveEpisode = [%graphql
  {|
  mutation($userId: String!, $episodeId: String!) {
  delete_my_episodes(where: {userId: {_eq: $userId}, episodeId: {_eq: $episodeId}}) {
    returning {
      episodeId
    }
  }
}
|}
];

module EpisodeMutation = ReasonApollo.CreateMutation(RemoveEpisode);

let getRemovedEpisodeId = (mutationResult: EpisodeMutation.mutationResult) =>
  mutationResult##data
  ->Belt.Option.flatMap(result => result##delete_my_episodes)
  ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0));

let makeEpisodeMutation = (~episodeId, ~userId) =>
  RemoveEpisode.make(~userId, ~episodeId, ());

let removeEpisodeFromCache = (~podcastId, ~userId, client, mutationResult) => {
  let removedId = getRemovedEpisodeId(mutationResult);

  switch (removedId) {
  | None => ()
  | Some(idObj) =>
    let updateCache = cache => {
      let myEpisodes =
        cache##my_episodes
        ->Belt.Array.keep(obj => obj##episodeId !== idObj##episodeId);
      LibraryCache.mergeIdsCache(~cache, ~myEpisodes, ());
    };

    LibraryCache.updateMyLibrarySavedIds(client, updateCache, userId);

    // remove from my library
    let updateLibraryCache = podcasts =>
      podcasts->Belt.Array.map(obj =>
        obj##listennotesId !== podcastId
          ? obj : MyLibrary.updatePodcastEpisodeCount(obj, count => count - 1)
      );
    LibraryCache.updateMyLibraryCache(client, updateLibraryCache, userId);
  };
};

let runEpisodeMutation =
    (
      ~mutation: EpisodeMutation.apolloMutation,
      ~episodeId,
      ~podcastId,
      ~userId,
    ) => {
  let removeEpisodeMutation = makeEpisodeMutation(~episodeId, ~userId);

  mutation(
    ~variables=removeEpisodeMutation##variables,
    ~update=removeEpisodeFromCache(~podcastId, ~userId),
    (),
  )
  |> ignore;
};

/** podcast */
module RemovePodcast = [%graphql
  {|
  mutation($userId: String!, $podcastId: String!) {
    delete_my_podcasts(where: {userId: {_eq: $userId}, podcastId: {_eq: $podcastId}}) {
    returning {
      podcastId
    }
  }
}
|}
];

module PodcastMutation = ReasonApollo.CreateMutation(RemovePodcast);

let makePodcastMutation = (~podcastId, ~userId) =>
  RemovePodcast.make(~userId, ~podcastId, ());

let getRemovedPodcastId = (mutationResult: PodcastMutation.mutationResult) =>
  mutationResult##data
  ->Belt.Option.flatMap(result => result##delete_my_podcasts)
  ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0));

let removePodcastFromCache = (~userId, client, mutationResult) => {
  let removedId = getRemovedPodcastId(mutationResult);

  switch (removedId) {
  | None => ()
  | Some(idObj) =>
    let updateIdsCache = cache => {
      let myPodcasts =
        cache##my_podcasts
        ->Belt.Array.keep(obj => obj##podcastId !== idObj##podcastId);
      LibraryCache.mergeIdsCache(~cache, ~myPodcasts, ());
    };
    LibraryCache.updateMyLibrarySavedIds(client, updateIdsCache, userId);

    // remove from my library
    let updateLibraryCache = podcasts =>
      podcasts->Belt.Array.keep(obj =>
        obj##listennotesId !== idObj##podcastId
      );
    LibraryCache.updateMyLibraryCache(client, updateLibraryCache, userId);
  };
};

let runPodcastMutation =
    (~mutation: PodcastMutation.apolloMutation, ~podcastId, ~userId) => {
  let removeEpisodeMutation = makePodcastMutation(~podcastId, ~userId);

  mutation(
    ~variables=removeEpisodeMutation##variables,
    ~update=removePodcastFromCache(~userId),
    (),
  )
  |> ignore;
};
