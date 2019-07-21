module SaveEpisode = [%graphql
  {|
    mutation(
      $title: String!,
      $pubDate: String!,
      $podcastTitle: String!,
      $podcastItunesId: String!,
      $podcastListennotesId: String!,
      $listennotesId: String!,
      $lengthSec: Int!,
      $itunesId: String,
      $genreIds: String!,
      $description: String!,
      $status: String!,
      $userId: String!,
      $tags: String!
   ) {
     insert_episodes(
       objects: [{
          title: $title,
          pubDate: $pubDate,
          podcastTitle: $podcastTitle,
          podcastListennotesId: $podcastListennotesId,
          podcastItunesId: $podcastItunesId,
          listennotesId: $listennotesId,
          lengthSec: $lengthSec,
          itunesId: $itunesId,
          description: $description
          genreIds: $genreIds,
          myEpisodes: {
            data: { tags: $tags, status: $status, userId: $userId },
            on_conflict: { constraint: my_episodes_pkey, update_columns: [] }
          }
       }],
       on_conflict: {
          constraint: episodes_listennotes_id_key,
          update_columns: [title, description, itunesId, pubDate]
       }) {
        returning {
          id
        }
      }
    }
  |}
];

let saveEpisode =
    (episode: EpisodeSearch.episode, data: MyLibrary.saveEpisodeData) => {
  SaveEpisode.make(
    ~title=episode.title,
    ~pubDate=episode.pubDate,
    ~podcastTitle=episode.podcastTitle,
    ~podcastListennotesId=episode.podcastListennotesId,
    ~listennotesId=episode.listennotesId,
    ~lengthSec=episode.lengthSec,
    //~itunesId=string_of_int(episode.podcastItunesId),
    ~description=episode.description,
    ~status=MyLibrary.statusEncoder(data.status),
    ~genreIds=Js.Array.joinWith(", ", episode.genreIds),
    ~podcastItunesId=string_of_int(episode.podcastItunesId),
    ~tags=data.tags,
    ~userId="margaretkru",
    (),
  );
};
