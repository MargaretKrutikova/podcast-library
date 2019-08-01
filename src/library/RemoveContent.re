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

let getRemovedPodcastId = (mutationResult: PodcastMutation.mutationResult) =>
  mutationResult##data
  ->Belt.Option.flatMap(result => result##delete_my_podcasts)
  ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0));

let makePodcastMutation = (~podcastId) =>
  RemovePodcast.make(~userId="margaretkru", ~podcastId, ());
