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

module Mutation = ReasonApollo.CreateMutation(SaveEpisode);

let getSavedId = (mutationResult: Mutation.mutationResult) =>
  mutationResult##data
  ->Belt.Option.flatMap(result => result##insert_episodes)
  ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0))
  ->Belt.Option.flatMap(result => result##myEpisodes->Belt.Array.get(0));

type episodeInsertInfo = {
  itunesId: option(string),
  podcastDescription: string,
  podcastImage: string,
};

type myEpisodeData = {
  status: EpisodeStatus.t,
  tags: string,
};

let makeMutation =
    (
      ~userId,
      ~episode: SearchResult.episode,
      ~libraryData: myEpisodeData,
      ~episodeInfo,
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
    ~status=EpisodeStatus.encode(libraryData.status),
    ~genreIds=Utils.toApiGenres(episode.genreIds),
    ~podcastItunesId=string_of_int(episode.podcastItunesId),
    ~tags=libraryData.tags,
    ~userId,
    ~publisher=episode.publisher,
    (),
  );
};

/** episode insert info */
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

let addEpisodeToCache = (~podcastId, ~userId, client, mutationResult) => {
  let insertedId = getSavedId(mutationResult);

  switch (insertedId) {
  | None => ()
  | Some(idObj) =>
    let updateCache = cache => {
      let myEpisodes = cache##my_episodes->Belt.Array.concat([|idObj|]);
      LibraryCache.mergeIdsCache(~cache, ~myEpisodes, ());
    };

    LibraryCache.updateMyLibrarySavedIds(client, updateCache, userId);

    // remove from my library
    let updateLibraryCache = podcasts =>
      podcasts->Belt.Array.map(obj =>
        obj##listennotesId !== podcastId
          ? obj : MyLibrary.updatePodcastEpisodeCount(obj, count => count + 1)
      );
    LibraryCache.updateMyLibraryCache(client, updateLibraryCache, userId);
  };
  ();
};
