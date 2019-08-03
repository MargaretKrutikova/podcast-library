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

let makeEpisodeMutation = (~episodeId) =>
  RemoveEpisode.make(~userId="margaretkru", ~episodeId, ());

let removeEpisodeFromCache = (podcastId, client, mutationResult) => {
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

    LibraryCache.updateMyLibrarySavedIds(client, updateCache);

    // remove from my library
    let updateLibraryCache = podcasts =>
      podcasts->Belt.Array.map(obj =>
        obj##listennotesId !== podcastId
          ? obj : MyLibrary.updatePodcastEpisodeCount(obj, count => count - 1)
      );
    LibraryCache.updateMyLibraryCache(client, updateLibraryCache);
  };
};

let runEpisodeMutation =
    (~mutation: EpisodeMutation.apolloMutation, ~episodeId, ~podcastId) => {
  let removeEpisodeMutation = makeEpisodeMutation(~episodeId);

  mutation(
    ~variables=removeEpisodeMutation##variables,
    ~update=removeEpisodeFromCache(podcastId),
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

let makePodcastMutation = (~podcastId) =>
  RemovePodcast.make(~userId="margaretkru", ~podcastId, ());

let getRemovedPodcastId = (mutationResult: PodcastMutation.mutationResult) =>
  mutationResult##data
  ->Belt.Option.flatMap(result => result##delete_my_podcasts)
  ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0));

let removePodcastFromCache = (client, mutationResult) => {
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
    LibraryCache.updateMyLibrarySavedIds(client, updateIdsCache);

    // remove from my library
    let updateLibraryCache = podcasts =>
      podcasts->Belt.Array.keep(obj =>
        obj##listennotesId !== idObj##podcastId
      );
    LibraryCache.updateMyLibraryCache(client, updateLibraryCache);
  };
};

let runPodcastMutation = (mutation: PodcastMutation.apolloMutation, podcastId) => {
  let removeEpisodeMutation = makePodcastMutation(~podcastId);

  mutation(
    ~variables=removeEpisodeMutation##variables,
    ~update=removePodcastFromCache,
    (),
  )
  |> ignore;
};
