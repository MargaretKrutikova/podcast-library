module SaveEpisode = [%graphql
  {|
    mutation(
      $title: String!,
      $pub_date_ms: Int!,
      $podcast_title: String!,
      $podcast_listennotes_id: String!,
      $listennotes_id: String!,
      $length_sec: Int!,
      $itunes_id: String!,
      $genre_ids: String!,
      $description: String!,
      $status: String!,
      $user_id: String!,
      $tags: String!
   ) {
     insert_episodes(
       objects: [{
          title: $title,
          pub_date_ms: $pub_date_ms,
          podcast_title: $podcast_title,
          podcast_listennotes_id: $podcast_listennotes_id,
          listennotes_id: $listennotes_id,
          length_sec: $length_sec,
          itunes_id: $itunes_id,
          description: $description
          genre_ids: $genre_ids,
          my_episodes: {
            data: { tags: $tags, status: $status, user_id: $user_id },
            on_conflict: { constraint: my_episodes_pkey, update_columns: [] }
          }
       }],
       on_conflict: {
          constraint: episodes_listennotes_id_key,
          update_columns: [title, description, itunes_id, pub_date_ms]
       }) {
        returning {
          id
        }
      }
    }
  |}
];

let saveEpisode =
    (episode: EpisodeResult.t, data: LibraryEpisode.saveEpisodeData) => {
  SaveEpisode.make(
    ~title=episode.title,
    ~pub_date_ms=0,
    ~podcast_title=episode.podcastTitle,
    ~podcast_listennotes_id=episode.podcastId,
    ~listennotes_id=episode.id,
    ~length_sec=episode.lengthSec,
    ~itunes_id=string_of_int(episode.itunesId),
    ~description=episode.description,
    ~status="NOT_LISTENED",
    ~genre_ids="",
    ~tags=data.tags,
    ~user_id="margaretkru",
    (),
  );
};
