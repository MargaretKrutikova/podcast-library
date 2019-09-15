open LibraryTypes;

/** episode */
module SaveEpisode = [%graphql
  {|
  mutation(
    $title: String!,
    $pubDate: String!,
    $podcastTitle: String!,
    $podcastDescription: String!,
    $podcastImage: String!,
    $publisher: String!,
    $podcastItunesId: String!,
    $podcastListennotesId: String!,
    $listennotesId: String!,
    $lengthSec: Int!,
    $itunesId: String,
    $genreIds: String!,
    $description: String!,
    $status: String!,
    $userId: String!,
    $tags: String!,
 ) {
   insert_episodes(
     objects: {
        title: $title,
        pubDate: $pubDate,
        listennotesId: $listennotesId,
        lengthSec: $lengthSec,
        itunesId: $itunesId,
        description: $description
        podcast: {
          data: {
              listennotesId: $podcastListennotesId, title: $podcastTitle, description: $podcastDescription,
              publisher: $publisher, itunesId: $podcastItunesId, genreIds: $genreIds, image: $podcastImage
          },
          on_conflict: {constraint: podcasts_pkey, update_columns: [genreIds]}
        }
        myEpisodes: {
          data: { tags: $tags, status: $status, userId: $userId },
          on_conflict: { constraint: my_episodes_pkey, update_columns: [] }
        }
     },
     on_conflict: {
        constraint: episodes_listennotes_id_key,
        update_columns: [title, description, itunesId, pubDate]
     }) {
      returning {
        myEpisodes {
          episodeId
        }
      }
    }
  }
|}
];

let makeSaveEpisodeMutation =
    (
      ~userId,
      ~episode: SearchTypes.episode,
      ~myEpisode: LibraryTypes.myEpisodeData,
      ~episodeInfo,
    ) => {
  SaveEpisode.make(
    ~title=episode.title,
    ~pubDate=episode.pubDate,
    ~podcastTitle=episode.podcastTitle,
    ~podcastDescription=episodeInfo.podcastDescription,
    ~podcastImage=episodeInfo.podcastImage,
    ~itunesId=?episodeInfo.itunesId,
    ~podcastListennotesId=episode.podcastId,
    ~listennotesId=episode.id,
    ~lengthSec=episode.lengthSec,
    ~description=episode.description,
    ~status=EpisodeStatus.encode(myEpisode.status),
    ~genreIds=Utils.toApiGenres(episode.genreIds),
    ~podcastItunesId=string_of_int(episode.podcastItunesId),
    ~tags=myEpisode.tags,
    ~userId,
    ~publisher=episode.publisher,
    (),
  );
};

module GetEpisodeInsertInfo = [%graphql
  {|
    query($podcastItunesId: String!, $podcastListennotesId: String!, $episodeName: String!) {
      itunesEpisode (podcastId: $podcastItunesId, episodeName: $episodeName) {
        id
      }
      getPodcastById(podcastId: $podcastListennotesId) {
        description
        image
      }
    }
|}
];

let getEpisodeInsertInfo = (episode: SearchTypes.episode) => {
  GetEpisodeInsertInfo.make(
    ~podcastItunesId=string_of_int(episode.podcastItunesId),
    ~podcastListennotesId=episode.podcastId,
    ~episodeName=episode.title,
    (),
  )
  |> Graphql.sendQuery
  |> Js.Promise.(
       then_(response => {
         let itunesId = response##itunesEpisode##id;

         let podcastDescription =
           Belt.Option.mapWithDefault(response##getPodcastById, "", info =>
             info##description
           );

         let podcastImage =
           Belt.Option.mapWithDefault(response##getPodcastById, "", info =>
             info##image
           );

         let info = {itunesId, podcastDescription, podcastImage};
         info |> resolve;
       })
     )
  |> Js.Promise.(
       catch(_ =>
         {itunesId: None, podcastDescription: "", podcastImage: ""} |> resolve
       )
     );
};

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

/** podcast */
module SavePodcast = [%graphql
  {|
  mutation(
    $userId: String!,
    $tags: String!,
    $image: String!,
    $listennotesId: String!,
    $title: String!,
    $description: String!,
    $publisher: String!,
    $itunesId: String!,
    $genreIds: String!) {
    insert_my_podcasts(objects: {
      userId: $userId, tags: $tags,
      podcast: {
        data: {description: $description, genreIds: $genreIds, image: $image, itunesId: $itunesId, title: $title, publisher: $publisher, listennotesId: $listennotesId},
        on_conflict: {constraint: podcasts_pkey, update_columns: [itunesId]}
      }
    }) {
      returning {
        podcastId
      }
    }
  }
|}
];

let makeSavePodcastMutation = (~podcast: SearchTypes.podcast, ~userId) =>
  SavePodcast.make(
    ~userId,
    ~tags="",
    ~image=podcast.image,
    ~listennotesId=podcast.id,
    ~title=podcast.title,
    ~description=podcast.description,
    ~publisher=podcast.publisher,
    ~itunesId=string_of_int(podcast.podcastItunesId),
    ~genreIds=Utils.toApiGenres(podcast.genreIds),
    (),
  );

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
