module RemovePodcastMutation =
  ReasonApolloHooks.Mutation.Make(LibraryMutations.RemovePodcast);

let useRemovePodcast = (~userId, ~podcastId) => {
  let dispatch = AppCore.useDispatch();
  let handleRemoveError = _ =>
    dispatch(ShowNotification({text: "Failed to remove", type_: Danger}));

  let handleRemoveDone = _ =>
    dispatch(ShowNotification({text: "Removed from library", type_: Info}));

  let (removePodcastMutation, simple, _full) =
    RemovePodcastMutation.use(
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
