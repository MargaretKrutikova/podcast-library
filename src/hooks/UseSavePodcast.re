module SavePodcastMutation =
  ReasonApolloHooks.Mutation.Make(LibraryMutations.SavePodcast);

let useSavePodcast = (~userId, ~podcast) => {
  let dispatch = AppCore.useDispatch();
  let handleSaveError = _ =>
    dispatch(ShowNotification({text: "Failed to save", type_: Error}));

  let handleSaveDone = _ =>
    dispatch(ShowNotification({text: "Saved to library", type_: Success}));

  let (savePodcast, simple, _full) =
    SavePodcastMutation.use(
      ~update=
        (client, mutationResult) =>
          mutationResult##data
          ->Belt.Option.map(result =>
              LibraryCache.addPodcastToCache(~userId, client, result)
            )
          |> ignore,
      (),
    );

  let handleSave = () => {
    let mutation =
      LibraryMutations.makeSavePodcastMutation(~userId, ~podcast);
    savePodcast(~variables=mutation##variables, ())
    |> Js.Promise.(then_(_ => handleSaveDone() |> resolve))
    |> Js.Promise.(catch(_ => handleSaveError() |> resolve));
  };

  (handleSave, simple);
};
