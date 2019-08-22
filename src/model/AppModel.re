type message =
  | /** notification  */
    ShowNotification(AppNotifications.data)
  | HideNotification(int)
  | RemoveNotification(int)
  | OnUnauthorizedAccess
  | SetShowIdentityModal(bool)
  | EnteredSearchTerm(string)
  | SetContentType(ContentType.t)
  | SetSearchModelFromQuery(Routing.searchQuery);

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

let update = (model, message) => {
  switch (message) {
  | EnteredSearchTerm(searchTerm) => (
      model.search
      |> SearchModel.updateSearchTerm(searchTerm)
      |> updateSearchModel(model),
      None,
    )
  | SetContentType(searchType) => (
      model.search
      |> SearchModel.updateSearchType(searchType)
      |> updateSearchModel(model),
      None,
    )
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
  | HideNotification(id) => (
      model.notifications
      |> AppNotifications.hide(id)
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
  | SetSearchModelFromQuery({query}) =>
    let updatedModel =
      query !== model.search.baseQuery.searchTerm
        ? model.search
          |> SearchModel.updateSearchTerm(query)
          |> updateSearchModel(model)
        : model;
    (updatedModel, None);
  };
};

let initialState = (createInitModel(), None);
