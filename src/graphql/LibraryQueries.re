module GetMyEpisodes = [%graphql
  {|
    query($userId: String!, $podcastId: String!) {
      my_episodes(where: {episode: {podcast: {id: {_eq: $podcastId}}}, _and: {userId: {_eq: $userId}}}) {
        episodeId
        status @bsDecoder(fn: "EpisodeStatus.decode")
        tags
        episode {
          id
          podcastId
          itunesId
          title
          description
          pubDate
          lengthSec
        }
      }
      podcasts(limit: 1, where: {id: {_eq: $podcastId}}) {
          title
          description
          itunesId
        }
      }
  |}
];

module GetAllEpisodes = [%graphql
  {|
    query($userId: String!) {
      my_episodes(where: {userId: {_eq: $userId}}, order_by: {addedDate: desc}) {
        episodeId
        status @bsDecoder(fn: "EpisodeStatus.decode")
        tags
        addedDate
        episode {
          id
          podcastId
          itunesId
          title
          description
          pubDate
          lengthSec
          podcast {
            itunesId
          }
        }
      }
    }
|}
];

module GetAllPodcasts = [%graphql
  {|
    query($userId: String!) {
      my_podcasts(where: {userId: {_eq: $userId}}, order_by: {addedDate: desc}) {
        podcastId
        addedDate
        podcast {
          description
          itunesId
          title
          publisher
        }
      }
    }
|}
];

module GetMyLibrary = [%graphql
  {|
  query($userId: String!) {
    get_my_episodes_grouped_by_podcasts (args:{userid: $userId}) {
        title
        numberOfEpisodes
        lastEpisodeAddedDate
        podcastAddedDate
        id
        description
        itunesId
        publisher
      }
    }
  |}
];

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
