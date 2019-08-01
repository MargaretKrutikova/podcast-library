open MyLibrary;
external toSavedIds: Js.Json.t => GetMyLibrarySavedIds.t = "%identity";

// this is necessary to keep __typename apollo field on the root cache query
let mergeCacheJs:
  (GetMyLibrarySavedIds.t, GetMyLibrarySavedIds.t) => GetMyLibrarySavedIds.t = [%bs.raw
  {|
      function (prev, next) {
        return { ...prev, ...next };
      }
    |}
];

let mergeCache =
    (~cache: GetMyLibrarySavedIds.t, ~myPodcasts=?, ~myEpisodes=?, ())
    : GetMyLibrarySavedIds.t => {
  "my_podcasts": myPodcasts->Belt.Option.getWithDefault(cache##my_podcasts),
  "my_episodes": myEpisodes->Belt.Option.getWithDefault(cache##my_episodes),
};

let updateMyLibrarySavedIds =
    (client, updateCache: GetMyLibrarySavedIds.t => GetMyLibrarySavedIds.t) => {
  let fetchMyLibraryIds = makeGetSavedIdsQuery();
  let cachedResponse =
    GetMyLibrarySavedIdsReadQuery.readQuery(
      client,
      Utils.toReadQueryOptions(fetchMyLibraryIds),
    );

  switch (cachedResponse |> Js.Nullable.toOption) {
  | None => ()
  | Some(cachedIds) =>
    let savedIds = toSavedIds(cachedIds);

    let updatedCachedIds = updateCache(savedIds);
    GetMyLibrarySavedIdsWriteQuery.make(
      ~client,
      ~variables=fetchMyLibraryIds##variables,
      ~data=updatedCachedIds |> mergeCacheJs(savedIds),
      (),
    );
  };
};
