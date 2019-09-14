type message =
  | ShowNotification(AppNotifications.data)
  | RemoveNotification(int)
  | OnUnauthorizedAccess
  | SetShowIdentityModal(bool)
  | EnteredSearchTerm(string)
  | SetContentType(ContentType.t)
  | SetEpisodeQueryPodcast(string)
  | SetSearchModelFromQuery(SearchQuery.t);

type model = {
  search: SearchQuery.t,
  notifications: AppNotifications.t,
  showIdentityModal: bool,
};

let createInitModel = () => {
  search: SearchQuery.make(),
  notifications: AppNotifications.init(),
  showIdentityModal: false,
};

let updateSearchModel = (model, search) => {...model, search};
let updateSearchTerm = (searchTerm, searchQuery: SearchQuery.t) => {
  ...searchQuery,
  searchTerm,
};
let updateSearchType = (searchType, searchQuery: SearchQuery.t) => {
  ...searchQuery,
  searchType,
};
let updateNotifications = (model, notifications) => {
  ...model,
  notifications,
};

let pushSearchQuery = (model, _) => {
  Routing.pushRoute(Search(model.search));
  None;
};

let update = (model, message) => {
  switch (message) {
  | EnteredSearchTerm(searchTerm) =>
    let model =
      model.search
      |> updateSearchTerm(searchTerm)
      |> updateSearchModel(model);
    (model, Some(pushSearchQuery(model)));
  | SetContentType(searchType) =>
    let model =
      model.search
      |> updateSearchType(searchType)
      |> updateSearchModel(model);
    (model, Some(pushSearchQuery(model)));
  | SetEpisodeQueryPodcast(podcastId) =>
    let model =
      {...model.search, podcastId: Some(podcastId)}
      |> updateSearchType(Episode)
      |> updateSearchModel(model);
    (model, Some(pushSearchQuery(model)));
  | OnUnauthorizedAccess => ({...model, showIdentityModal: true}, None)
  | SetShowIdentityModal(showIdentityModal) => (
      {...model, showIdentityModal},
      None,
    )
  | ShowNotification(data) => (
      model.notifications
      |> AppNotifications.add(data)
      |> updateNotifications(model),
      None,
    )
  | RemoveNotification(id) => (
      {
        ...model,
        notifications: model.notifications |> AppNotifications.remove(id),
      },
      None,
    )
  | SetSearchModelFromQuery(searchQuery) =>
    let model = {...model, search: searchQuery};
    (model, None);
  };
};

let initEffect = dispatch => {
  let route =
    ReasonReact.Router.dangerouslyGetInitialUrl() |> Routing.urlToRoute;
  switch (route) {
  | Search(query) => dispatch(SetSearchModelFromQuery(query))
  | _ => ignore()
  };
  None;
};

let initialState = (createInitModel(), Some(initEffect));
