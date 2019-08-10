type message =
  | /** notification  */
    ShowNotification(AppNotifications.data)
  | HideNotification(int)
  | RemoveNotification(int)
  | GotSearchMessage(SearchModel.message)
  | OnUnauthorizedAccess
  | SetShowIdentityModal(bool);

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
  | GotSearchMessage(subMessage) =>
    let (search, effect) = SearchModel.update(model.search, subMessage);
    ({...model, search}, effect);
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
