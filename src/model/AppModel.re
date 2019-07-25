/** my library model */

type myLibrary =
  | NotAsked
  | Loading(option(MyLibrary.library))
  | Loaded(MyLibrary.library);

type message =
  | /** notification  */
    ShowNotification(AppNotifications.data)
  | HideNotification(int)
  | RemoveNotification(int)
  | GotSearchMessage(SearchModel.message)
  | /** library */
    FetchLibraryIds
  | GotLibraryIds(MyLibrary.libraryIds)
  | FetchLibrary
  | GotLibrary(MyLibrary.library)
  | SavedEpisode(SearchResult.episode)
  | SavedPodcast(SearchResult.podcast);

type model = {
  search: SearchModel.t,
  libraryIds: MyLibrary.libraryIds,
  myLibrary,
  notifications: AppNotifications.t,
};

let createInitModel = () => {
  search: SearchModel.createInitModel(),
  libraryIds: {
    episodes: [||],
    podcasts: [||],
  },
  myLibrary: NotAsked,
  notifications: AppNotifications.init(),
};

let toLoading = (library: myLibrary): myLibrary => {
  switch (library) {
  | NotAsked => Loading(None)
  | Loaded(data) => Loading(Some(data))
  | other => other
  };
};

/** effects */

let getLibraryIds = ((), dispatch) => {
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

let addEpisodeId = (libraryIds: MyLibrary.libraryIds, episodeId) => {
  ...libraryIds,
  episodes: Belt.Array.concat(libraryIds.episodes, [|episodeId|]),
};

let addPodcastId = (libraryIds: MyLibrary.libraryIds, podcastId) => {
  ...libraryIds,
  podcasts: Belt.Array.concat(libraryIds.podcasts, [|podcastId|]),
};

/** notifications */

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
  | GotLibraryIds(ids) => ({...model, libraryIds: ids}, None)
  | SavedEpisode(episode) => (
      {
        ...model,
        libraryIds: addEpisodeId(model.libraryIds, episode.listennotesId),
      },
      None,
    )
  | SavedPodcast(podcast) => (
      {
        ...model,
        libraryIds: addPodcastId(model.libraryIds, podcast.listennotesId),
      },
      None,
    )
  | ShowNotification(data) => (
      {
        ...model,
        notifications: model.notifications |> AppNotifications.add(data),
      },
      None,
    )
  | HideNotification(id) => (
      {
        ...model,
        notifications: model.notifications |> AppNotifications.hide(id),
      },
      None,
    )
  | RemoveNotification(id) => (
      {
        ...model,
        notifications: model.notifications |> AppNotifications.remove(id),
      },
      None,
    )
  };
};

let initialState = (createInitModel(), None);
