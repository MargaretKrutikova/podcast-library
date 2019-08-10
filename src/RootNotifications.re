open BsReactstrap;

let getNotifications = (model: AppCore.model) => model.notifications.items;

[@react.component]
let make = () => {
  let notifications = AppCore.useSelector(getNotifications);
  let containerRef = React.useRef(Js.Nullable.null);

  let style =
    ReactDOMRe.Style.make(
      ~top=int_of_float(Utils.scrollTop) < 10 ? "55px" : "20px",
      (),
    );

  <div ref={ReactDOMRe.Ref.domRef(containerRef)}>
    <div
      style
      className="container app-notifications-root col-lg-4 col-md-6 col-sm-8 col-xs-12">
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
    </div>
  </div>;
};
