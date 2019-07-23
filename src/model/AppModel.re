/** my library model */

type myLibrary =
  | NotAsked
  | Loading(option(MyLibrary.library))
  | Loaded(MyLibrary.library);

type message =
  | GotSearchMessage(SearchModel.message)
  | /** library */
    FetchLibraryIds
  | GotLibraryIds(array(string))
  | FetchLibrary
  | GotLibrary(MyLibrary.library)
  | SavedEpisode(SearchResult.episode);

type model = {
  search: SearchModel.t,
  librarySavedIds: array(string),
  myLibrary,
};

let createInitModel = () => {
  search: SearchModel.createInitModel(),
  librarySavedIds: [||],
  myLibrary: NotAsked,
};

let toLoading = (library: myLibrary): myLibrary => {
  switch (library) {
  | NotAsked => Loading(None)
  | Loaded(data) => Loading(Some(data))
  | other => other
  };
};

/** effects */

let getLibraryIds = (model, dispatch) => {
  MyLibrary.getSavedIds()
  |> Js.Promise.(then_(ids => dispatch(GotLibraryIds(ids)) |> resolve))
  |> ignore;

  None;
};

let getMyLibrary = ((), dispatch) => {
  MyLibrary.getFull()
  |> Js.Promise.(
       then_(episodes =>
         dispatch(GotLibrary({episodes: episodes})) |> resolve
       )
     )
  |> ignore;

  None;
};

let update = (model, message) => {
  switch (message) {
  | GotSearchMessage(subMessage) =>
    let (search, effect) =
      SearchModel.update(model.search, subMessage, message =>
        GotSearchMessage(message)
      );
    ({...model, search}, effect);
  | FetchLibraryIds => (model, Some(getLibraryIds()))
  | FetchLibrary => (
      {...model, myLibrary: toLoading(model.myLibrary)},
      Some(getMyLibrary()),
    )
  | GotLibrary(myLibrary) => (
      {...model, myLibrary: Loaded(myLibrary)},
      None,
    )
  | GotLibraryIds(ids) => ({...model, librarySavedIds: ids}, None)
  | SavedEpisode(episode) => (
      {
        ...model,
        librarySavedIds:
          Belt.Array.concat(
            model.librarySavedIds,
            [|episode.listennotesId|],
          ),
      },
      None,
    )
  };
};

let initialState = (createInitModel(), None);
