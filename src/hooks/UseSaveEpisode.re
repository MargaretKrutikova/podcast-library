module SaveEpisodeMutation =
  ReasonApolloHooks.Mutation.Make(LibraryMutations.SaveEpisode);

let getSavedEpisodeId = (mutationResult: SaveEpisodeMutation.mutationResult) =>
  mutationResult##data
  ->Belt.Option.flatMap(result => result##insert_episodes)
  ->Belt.Option.flatMap(result => result##returning->Belt.Array.get(0))
  ->Belt.Option.flatMap(result => result##myEpisodes->Belt.Array.get(0));

let addEpisodeToCache = (~userId, client, mutationResult) => {
  let insertedId = getSavedEpisodeId(mutationResult);

  switch (insertedId) {
  | None => ()
  | Some(idObj) =>
    // update cache with ids
    let updateCache = cache => {
      let myEpisodes = cache##my_episodes->Belt.Array.concat([|idObj|]);
      LibraryCache.mergeIdsCache(~cache, ~myEpisodes, ());
    };

    LibraryCache.updateMyLibrarySavedIds(client, updateCache, userId);
  };
};

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
    dispatch(ShowNotification({text: "Failed to save", type_: Danger}));

  let handleSaveDone = _ =>
    dispatch(ShowNotification({text: "Saved to library", type_: Info}));

  let (fetchEpisodeResult, setFetchResult) = React.useState(() => NotCalled);

  let (saveEpisodeMutation, simple, _full) =
    SaveEpisodeMutation.use(
      ~refetchQueries=
        _ => {
          let query = LibraryQueries.GetMyLibrary.make(~userId, ());
          [|ReasonApolloHooks.Utils.toQueryObj(query)|];
        },
      ~update=addEpisodeToCache(~userId),
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
