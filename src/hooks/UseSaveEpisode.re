module SaveEpisodeMutation =
  ReasonApolloHooks.Mutation.Make(LibraryMutations.SaveEpisode);

type fetchResult =
  | NotCalled
  | Fetching
  | Success
  | Failure;

type saveEpisodeResult('a) =
  | Loading
  | Error
  | NoData
  | Data('a);

let useSaveEpisode = (~userId, ~episode: SearchTypes.episode) => {
  let dispatch = AppCore.useDispatch();
  let handleSaveError = _ =>
    dispatch(ShowNotification({text: "Failed to save", type_: Error}));

  let handleSaveDone = _ =>
    dispatch(ShowNotification({text: "Saved to library", type_: Success}));

  let (fetchEpisodeResult, setFetchResult) = React.useState(() => NotCalled);

  let (saveEpisodeMutation, simple, _full) =
    SaveEpisodeMutation.use(
      ~update=
        (client, mutationResult) =>
          mutationResult##data
          ->Belt.Option.map(result =>
              LibraryCache.addEpisodeToCache(~userId, client, result)
            )
          |> ignore,
      (),
    );

  let callMutation = episodeInfo => {
    let myEpisode: LibraryTypes.myEpisodeData = {
      tags: "",
      status: NotListened,
    };
    let mutation =
      LibraryMutations.makeSaveEpisodeMutation(
        ~userId,
        ~episode,
        ~myEpisode,
        ~episodeInfo,
      );
    saveEpisodeMutation(~variables=mutation##variables, ());
  };

  let handleSave = () => {
    setFetchResult(_ => Fetching);

    LibraryMutations.getEpisodeInsertInfo(episode)
    |> Js.Promise.(
         then_(episodeInfo => {
           setFetchResult(_ => Success);

           callMutation(episodeInfo)
           |> then_(_ => handleSaveDone() |> resolve)
           |> catch(_ => handleSaveError() |> resolve);
         })
       )
    |> Js.Promise.(
         catch(_ => {
           setFetchResult(_ => Failure);
           handleSaveError() |> resolve;
         })
       );
  };

  let mutationResult =
    switch (simple, fetchEpisodeResult) {
    | (Data(d), Success) => Data(d)
    | (Loading, _)
    | (_, Fetching) => Loading
    | (Error(_), _)
    | (_, Failure) => Error
    | (NoData, NotCalled) => NoData
    | (_, _) => NoData
    };

  (handleSave, mutationResult);
};
