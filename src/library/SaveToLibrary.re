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
          listennotesId
        }
      }
    }
  |}
];

module UpdateItunesId = [%graphql
  {|
  mutation($listennotesId: String!, $itunesId: String!) {
    update_episodes(where: {listennotesId: {_eq: $listennotesId}}, _set: {itunesId: $itunesId}) {
      affected_rows
    }
  }
|}
];

module GetEpisodeItunesId = [%graphql
  {|
    query($podcastItunesId: String!, $episodeName: String!) {
      itunesEpisode (podcastId: $podcastItunesId, episodeName: $episodeName) {
        id
      }
    }
|}
];

let updateEpisodeItunesId = (~podcastItunesId, ~episodeId, ~episodeName) => {
  GetEpisodeItunesId.make(~podcastItunesId, ~episodeName, ())
  |> Graphql.sendQuery
  |> Js.Promise.(
       then_(response => {
         let itunesId = response##itunesEpisode##id;

         switch (itunesId) {
         | None => resolve(0)
         | Some(id) =>
           UpdateItunesId.make(~listennotesId=episodeId, ~itunesId=id, ())
           |> Graphql.sendQuery
           |> then_(response =>
                switch (response##update_episodes) {
                | None => resolve(0)
                | Some(res) => resolve(res##affected_rows)
                }
              )
         };
       })
     );
};

let performEpisodeSave =
    (
      episode: EpisodeSearch.episode,
      data: MyLibrary.saveEpisodeData,
      itunesId,
    ) => {
  SaveEpisode.make(
    ~title=episode.title,
    ~pubDate=episode.pubDate,
    ~podcastTitle=episode.podcastTitle,
    ~podcastListennotesId=episode.podcastListennotesId,
    ~listennotesId=episode.listennotesId,
    ~lengthSec=episode.lengthSec,
    ~itunesId,
    ~description=episode.description,
    ~status=MyLibrary.statusEncoder(data.status),
    ~genreIds=Js.Array.joinWith(", ", episode.genreIds),
    ~podcastItunesId=string_of_int(episode.podcastItunesId),
    ~tags=data.tags,
    ~userId="margaretkru",
    (),
  )
  |> Graphql.sendQuery
  |> Js.Promise.then_(response =>
       response##insert_episodes |> Js.Promise.resolve
     );
};

let saveEpisode =
    (episode: EpisodeSearch.episode, data: MyLibrary.saveEpisodeData) => {
  GetEpisodeItunesId.make(
    ~podcastItunesId=string_of_int(episode.podcastItunesId),
    ~episodeName=episode.title,
    (),
  )
  |> Graphql.sendQuery
  |> Js.Promise.(
       then_(response => {
         let itunesId =
           switch (response##itunesEpisode##id) {
           | None => ""
           | Some(id) => id
           };
         performEpisodeSave(episode, data, itunesId);
       })
     )
  |> Js.Promise.(catch(_ => performEpisodeSave(episode, data, "")));
};
