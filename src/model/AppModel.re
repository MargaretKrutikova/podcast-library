type message =
  | /** notification  */
    ShowNotification(AppNotifications.data)
  | HideNotification(int)
  | RemoveNotification(int)
  | GotSearchMessage(SearchModel.message)
  | /** library */
    FetchLibraryIds
  | GotLibraryIds(MyLibrary.libraryIds)
  | SavedEpisode(SearchResult.episode)
  | SavedPodcast(SearchResult.podcast);

type model = {
  search: SearchModel.t,
  libraryIds: MyLibrary.libraryIds,
  notifications: AppNotifications.t,
};

let createInitModel = () => {
  search: SearchModel.createInitModel(),
  libraryIds: {
    episodes: [||],
    podcasts: [||],
  },
  notifications: AppNotifications.init(),
};

/** effects */

let getLibraryIds = ((), dispatch) => {
  MyLibrary.getSavedIds()
  |> Js.Promise.(then_(ids => dispatch(GotLibraryIds(ids)) |> resolve))
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
    let (search, effect) = SearchModel.update(model.search, subMessage);
    ({...model, search}, effect);
  | FetchLibraryIds => (model, Some(getLibraryIds()))
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
