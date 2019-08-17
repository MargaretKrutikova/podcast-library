module RemoveEpisodeMutation =
  ReasonApolloHooks.Mutation.Make(LibraryMutations.RemoveEpisode);

let getRemovedEpisodeId =
    (mutationResult: RemoveEpisodeMutation.mutationResult) =>
  mutationResult##data
  ->Belt.Option.flatMap(result => result##delete_my_episodes)
  ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0));

let removeEpisodeFromCache = (~userId, client, mutationResult) => {
  let removedId = getRemovedEpisodeId(mutationResult);

  switch (removedId) {
  | None => ()
  | Some(idObj) =>
    let updateCache = cache => {
      let myEpisodes =
        cache##my_episodes
        ->Belt.Array.keep(obj => obj##episodeId !== idObj##episodeId);
      LibraryCache.mergeIdsCache(~cache, ~myEpisodes, ());
    };

    LibraryCache.updateMyLibrarySavedIds(client, updateCache, userId);
  };
};

let useRemoveEpisode = (~userId, ~episodeId, ~refetchQueries=?, ()) => {
  let dispatch = AppCore.useDispatch();
  let handleRemoveError = _ =>
    dispatch(ShowNotification({text: "Failed to remove", type_: Danger}));

  let handleRemoveDone = _ =>
    dispatch(ShowNotification({text: "Removed from library", type_: Info}));

  let queries =
    switch (refetchQueries) {
    | None =>
      let query = LibraryQueries.GetMyLibrary.make(~userId, ());
      [|ReasonApolloHooks.Utils.toQueryObj(query)|];
    | Some(qs) => qs
    };
  let (removeEpisodeMutation, simple, _full) =
    RemoveEpisodeMutation.use(
      ~refetchQueries=_ => queries,
      ~update=removeEpisodeFromCache(~userId),
      (),
    );

  let handleRemove = () => {
    let mutation =
      LibraryMutations.RemoveEpisode.make(~episodeId, ~userId, ());
    removeEpisodeMutation(~variables=mutation##variables, ())
    |> Js.Promise.(then_(_ => handleRemoveDone() |> resolve))
    |> Js.Promise.(catch(_ => handleRemoveError() |> resolve));
  };

  (handleRemove, simple);
};
