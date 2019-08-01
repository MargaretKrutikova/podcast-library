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

let removeEpisodeIdFromCache = (client, mutationResult) => {
  let removedId = getRemovedEpisodeId(mutationResult);

  switch (removedId) {
  | None => ()
  | Some(idObj) =>
    let updateCache = cache => {
      let myEpisodes =
        cache##my_episodes
        ->Belt.Array.keep(obj => obj##episodeId !== idObj##episodeId);
      LibraryCache.mergeCache(~cache, ~myEpisodes, ());
    };

    LibraryCache.updateMyLibrarySavedIds(client, updateCache);
  };
};

let runEpisodeMutation = (mutation: EpisodeMutation.apolloMutation, episodeId) => {
  let refetchMyLibraryQuery = MyLibrary.getMyLibraryQuery();
  let removeEpisodeMutation = makeEpisodeMutation(~episodeId);

  mutation(
    ~variables=removeEpisodeMutation##variables,
    ~refetchQueries=_ => [|Utils.toQueryObj(refetchMyLibraryQuery)|],
    ~update=removeEpisodeIdFromCache,
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

let removePodcastIdFromCache = (client, mutationResult) => {
  let removedId = getRemovedPodcastId(mutationResult);

  switch (removedId) {
  | None => ()
  | Some(idObj) =>
    let updateCache = cache => {
      let myPodcasts =
        cache##my_podcasts
        ->Belt.Array.keep(obj => obj##podcastId !== idObj##podcastId);
      LibraryCache.mergeCache(~cache, ~myPodcasts, ());
    };
    LibraryCache.updateMyLibrarySavedIds(client, updateCache);
  };
};

let runPodcastMutation = (mutation: PodcastMutation.apolloMutation, podcastId) => {
  let refetchMyLibraryQuery = MyLibrary.getMyLibraryQuery();
  let removeEpisodeMutation = makePodcastMutation(~podcastId);

  mutation(
    ~variables=removeEpisodeMutation##variables,
    ~refetchQueries=_ => [|Utils.toQueryObj(refetchMyLibraryQuery)|],
    ~update=removePodcastIdFromCache,
    (),
  )
  |> ignore;
};
