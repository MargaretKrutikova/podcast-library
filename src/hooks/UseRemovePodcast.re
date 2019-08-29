module RemovePodcastMutation =
  ReasonApolloHooks.Mutation.Make(LibraryMutations.RemovePodcast);

let useRemovePodcast = (~userId, ~podcastId, ~refetchQueries=?, ()) => {
  let dispatch = AppCore.useDispatch();
  let handleRemoveError = _ =>
    dispatch(ShowNotification({text: "Failed to remove", type_: Error}));

  let handleRemoveDone = _ =>
    dispatch(
      ShowNotification({text: "Removed from library", type_: Success}),
    );

  let (removePodcastMutation, simple, _full) =
    RemovePodcastMutation.use(
      ~refetchQueries=?
        refetchQueries->Belt.Option.map((queries, _) => queries),
      ~update=
        (client, mutationResult) =>
          mutationResult##data
          ->Belt.Option.map(result =>
              LibraryCache.removePodcastFromCache(~userId, client, result)
            )
          |> ignore,
      (),
    );

  let handleRemove = () => {
    let mutation =
      LibraryMutations.RemovePodcast.make(~podcastId, ~userId, ());
    removePodcastMutation(~variables=mutation##variables, ())
    |> Js.Promise.(then_(_ => handleRemoveDone() |> resolve))
    |> Js.Promise.(catch(_ => handleRemoveError() |> resolve));
  };

  (handleRemove, simple);
};
