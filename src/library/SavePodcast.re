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

module Mutation = ReasonApollo.CreateMutation(SavePodcast);

let getSavedId = (mutationResult: Mutation.mutationResult) =>
  mutationResult##data
  ->Belt.Option.flatMap(result => result##insert_my_podcasts)
  ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0));

let makeMutation = (~podcast: SearchResult.podcast, ~userId) =>
  SavePodcast.make(
    ~userId,
    ~tags="",
    ~image=podcast.image,
    ~listennotesId=podcast.listennotesId,
    ~title=podcast.title,
    ~description=podcast.description,
    ~publisher=podcast.publisher,
    ~itunesId=string_of_int(podcast.podcastItunesId),
    ~genreIds=Utils.toApiGenres(podcast.genreIds),
    (),
  );

let addPodcastIdToCache = (~userId, client, mutationResult) => {
  let insertedId = getSavedId(mutationResult);

  switch (insertedId) {
  | None => ()
  | Some(idObj) =>
    let updateCache = cache => {
      let myPodcasts = cache##my_podcasts->Belt.Array.concat([|idObj|]);
      LibraryCache.mergeIdsCache(~cache, ~myPodcasts, ());
    };

    LibraryCache.updateMyLibrarySavedIds(client, updateCache, userId);
  };
  ();
};
