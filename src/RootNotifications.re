open BsReactstrap;

let getNotifications = (model: AppCore.model) => model.notifications.items;

[@react.component]
let make = () => {
  let notifications = AppCore.useSelector(getNotifications);

  <Container className="app-notifications-root">
    {notifications->Belt.Array.map(item =>
       <Notification
         key={string_of_int(item.id)}
         id={item.id}
         isShown={item.isShown}
         type_={item.data.type_}>
         {ReasonReact.string(item.data.text)}
       </Notification>
     )
     |> ReasonReact.array}
  </Container>;
};
