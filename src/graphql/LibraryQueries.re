module GetMyEpisodes = [%graphql
  {|
    query($userId: String!, $podcastId: String!) {
      my_episodes(where: {episode: {podcast: {listennotesId: {_eq: $podcastId}}}, _and: {userId: {_eq: $userId}}}) {
        episodeId
        status @bsDecoder(fn: "EpisodeStatus.decode")
        tags
        episode {
          listennotesId
          podcastListennotesId
          itunesId
          title
          description
          pubDate
          lengthSec
          podcast {
            itunesId
            title
            image
            publisher
          }
        }
      }
      podcasts(limit: 1, where: {listennotesId: {_eq: $podcastId}}) {
          title
          description
          itunesId
          image
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
          listennotesId
          podcastListennotesId
          itunesId
          title
          description
          pubDate
          lengthSec
          podcast {
            itunesId
            title
            image
            publisher
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
          image
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
        listennotesId
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
