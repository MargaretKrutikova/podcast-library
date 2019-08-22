let maxNumberOfNotifications = 3;

type type_ =
  | Primary
  | Secondary
  | Success
  | Error
  | Warning
  | Info
  | Light
  | Dark;

type data = {
  text: string,
  type_,
};

type notification = {
  id: int,
  data,
};

type t = {
  items: array(notification),
  lastId: int,
};

let init = () => {items: [||], lastId: 0};

let remove = (id, notifications) => {
  let items = Belt.Array.keep(notifications.items, item => item.id != id);
  {...notifications, items};
};

let add = (data, notifications) => {
  let nextId = notifications.lastId + 1;

  let items =
    Belt.Array.concat(notifications.items, [|{id: nextId, data}|]);
  let numberOfShownItems = Belt.Array.length(items);

  let itemsToShow =
    numberOfShownItems > maxNumberOfNotifications
      ? Belt.Array.slice(
          items,
          ~offset=numberOfShownItems - maxNumberOfNotifications,
          ~len=maxNumberOfNotifications,
        )
      : items;

  {items: itemsToShow, lastId: nextId};
};
