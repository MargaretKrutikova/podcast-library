module RemoveEpisodeMutation =
  ReasonApolloHooks.Mutation.Make(LibraryMutations.RemoveEpisode);

let useRemoveEpisode = (~userId, ~episodeId, ~refetchQueries=?, ()) => {
  let dispatch = AppCore.useDispatch();
  let handleRemoveError = _ =>
    dispatch(ShowNotification({text: "Failed to remove", type_: Danger}));

  let handleRemoveDone = _ =>
    dispatch(ShowNotification({text: "Removed from library", type_: Info}));

  let (removeEpisodeMutation, simple, _full) =
    RemoveEpisodeMutation.use(
      ~refetchQueries=?
        refetchQueries->Belt.Option.map((queries, _) => queries),
      ~update=
        (client, mutationResult) =>
          mutationResult##data
          ->Belt.Option.map(result =>
              LibraryCache.removeEpisodeFromCache(~userId, client, result)
            )
          |> ignore,
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
