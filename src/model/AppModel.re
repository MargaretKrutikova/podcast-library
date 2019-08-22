type message =
  | /** notification  */
    ShowNotification(AppNotifications.data)
  | HideNotification(int)
  | RemoveNotification(int)
  | OnUnauthorizedAccess
  | SetShowIdentityModal(bool)
  | EnteredSearchTerm(string)
  | SetContentType(ContentType.t);

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

/** notifications */

let update = (model, message) => {
  switch (message) {
  | EnteredSearchTerm(searchTerm) => (
      {
        ...model,
        search: SearchModel.updateSearchTerm(model.search, searchTerm),
      },
      None,
    )
  | SetContentType(searchType) => (
      {
        ...model,
        search: SearchModel.updateSearchType(model.search, searchType),
      },
      None,
    )
  | OnUnauthorizedAccess => ({...model, showIdentityModal: true}, None)
  | SetShowIdentityModal(showIdentityModal) => (
      {...model, showIdentityModal},
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
