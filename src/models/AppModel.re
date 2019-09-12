type message =
  | ShowNotification(AppNotifications.data)
  | RemoveNotification(int)
  | OnUnauthorizedAccess
  | SetShowIdentityModal(bool)
  | EnteredSearchTerm(string)
  | SetContentType(ContentType.t)
  | SetSearchModelFromQuery(SearchQs.t);

type model = {
  search: SearchModel.t,
  notifications: AppNotifications.t,
  showIdentityModal: bool,
};

let createInitModel = () => {
  search: SearchModel.createInitModel(),
  notifications: AppNotifications.init(),
  showIdentityModal: false,
};

let updateSearchModel = (model, search) => {...model, search};
let updateNotifications = (model, notifications) => {
  ...model,
  notifications,
};

let pushSearchQuery = (model, _) => {
  let searchQuery = SearchModel.toSearchQuery(model.search);
  Routing.pushRoute(Search(searchQuery));
  None;
};

let update = (model, message) => {
  switch (message) {
  | EnteredSearchTerm(searchTerm) =>
    let model =
      model.search
      |> SearchModel.updateSearchTerm(searchTerm)
      |> updateSearchModel(model);
    (model, Some(pushSearchQuery(model)));
  | SetContentType(searchType) =>
    let model =
      model.search
      |> SearchModel.updateSearchType(searchType)
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
    let model = {...model, search: SearchModel.fromSearchQuery(searchQuery)};
    (model, None);
  };
};

let initialState = (createInitModel(), None);
