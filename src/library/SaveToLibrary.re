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
          listennotesId
        }
      }
    }
  |}
];

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
        on_conflict: {constraint: podcasts_pkey, update_columns: []}
      }
    }) {
      affected_rows
    }
  }
|}
];

let toGenres = (arr: array(int)) => Js.Array.joinWith(",", arr);

type episodeInsertInfo = {
  itunesId: option(string),
  podcastDescription: string,
  podcastImage: string,
};

let performEpisodeSave =
    (
      ~episode: SearchResult.episode,
      ~libraryData: MyLibrary.saveEpisodeData,
      ~episodeInfo,
      (),
    ) => {
  SaveEpisode.make(
    ~title=episode.title,
    ~pubDate=episode.pubDate,
    ~podcastTitle=episode.podcastTitle,
    ~podcastDescription=episodeInfo.podcastDescription,
    ~podcastImage=episodeInfo.podcastImage,
    ~itunesId=?episodeInfo.itunesId,
    ~podcastListennotesId=episode.podcastListennotesId,
    ~listennotesId=episode.listennotesId,
    ~lengthSec=episode.lengthSec,
    ~description=episode.description,
    ~status=MyLibrary.statusEncoder(libraryData.status),
    ~genreIds=toGenres(episode.genreIds),
    ~podcastItunesId=string_of_int(episode.podcastItunesId),
    ~tags=libraryData.tags,
    ~userId="margaretkru",
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

let getEpisodeInsertInfo = (episode: SearchResult.episode) => {
  GetEpisodeInsertInfo.make(
    ~podcastItunesId=string_of_int(episode.podcastItunesId),
    ~podcastListennotesId=episode.podcastListennotesId,
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

let savePodcastMutation = (podcast: SearchResult.podcast) =>
  SavePodcast.make(
    ~userId="margaretkru",
    ~tags="",
    ~image=podcast.image,
    ~listennotesId=podcast.listennotesId,
    ~title=podcast.title,
    ~description=podcast.description,
    ~publisher=podcast.publisher,
    ~itunesId=string_of_int(podcast.podcastItunesId),
    ~genreIds=toGenres(podcast.genreIds),
    (),
  );
