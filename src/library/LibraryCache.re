open MyLibrary;
external cast: Js.Json.t => 'a = "%identity";

/** library ids cache */
module GetMyLibrarySavedIdsReadQuery =
  ApolloClient.ReadQuery(GetMyLibrarySavedIds);

module GetMyLibrarySavedIdsWriteQuery =
  ApolloClient.WriteQuery(GetMyLibrarySavedIds);

// this is necessary to keep __typename apollo field on the root cache query
let mergeIdsCacheJs:
  (GetMyLibrarySavedIds.t, GetMyLibrarySavedIds.t) => GetMyLibrarySavedIds.t = [%bs.raw
  {|
      function (prev, next) {
        return { ...prev, ...next };
      }
    |}
];

let mergeIdsCache =
    (~cache: GetMyLibrarySavedIds.t, ~myPodcasts=?, ~myEpisodes=?, ())
    : GetMyLibrarySavedIds.t => {
  "my_podcasts": myPodcasts->Belt.Option.getWithDefault(cache##my_podcasts),
  "my_episodes": myEpisodes->Belt.Option.getWithDefault(cache##my_episodes),
};

let updateMyLibrarySavedIds =
    (client, updateCache: GetMyLibrarySavedIds.t => GetMyLibrarySavedIds.t) => {
  let fetchMyLibraryIds = makeGetSavedIdsQuery();
  switch (
    GetMyLibrarySavedIdsReadQuery.readQuery(
      client,
      Utils.toReadQueryOptions(fetchMyLibraryIds),
    )
  ) {
  | exception _ => ()
  | cachedResponse =>
    switch (cachedResponse |> Js.Nullable.toOption) {
    | None => ()
    | Some(cachedIds) =>
      let savedIds = cast(cachedIds);

      let updatedCachedIds = updateCache(savedIds);
      GetMyLibrarySavedIdsWriteQuery.make(
        ~client,
        ~variables=fetchMyLibraryIds##variables,
        ~data=updatedCachedIds |> mergeIdsCacheJs(savedIds),
        (),
      );
    }
  };
};

/** my library cache */
module GetMyLibraryReadQuery = ApolloClient.ReadQuery(GetMyLibrary);

module GetMyLibraryWriteQuery = ApolloClient.WriteQuery(GetMyLibrary);

let mergeLibraryCacheJs: (GetMyLibrary.t, GetMyLibrary.t) => GetMyLibrary.t = [%bs.raw
  {|
      function (prev, next) {
        return { ...prev, ...next };
      }
    |}
];

let updateMyLibraryCache = (client, updateCache) => {
  let fetchMyLibrary = makeGetMyLibraryQuery();
  switch (
    GetMyLibraryReadQuery.readQuery(
      client,
      Utils.toReadQueryOptions(fetchMyLibrary),
    )
  ) {
  | exception _ => ()
  | cachedResponse =>
    switch (cachedResponse |> Js.Nullable.toOption) {
    | None => ()
    | Some(data) =>
      let library = cast(data);

      let updatedLibrary = {
        "get_my_episodes_grouped_by_podcasts":
          updateCache(library##get_my_episodes_grouped_by_podcasts),
      };

      GetMyLibraryWriteQuery.make(
        ~client,
        ~variables=fetchMyLibrary##variables,
        ~data=updatedLibrary |> mergeLibraryCacheJs(updatedLibrary),
        (),
      );
    }
  };
  ();
};
