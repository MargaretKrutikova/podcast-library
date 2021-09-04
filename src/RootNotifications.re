let getNotifications = (model: AppModel.model) => model.notifications.items;

[@react.component]
let make = () => {
  let notifications = AppCore.useSelector(getNotifications);
  let containerRef = React.useRef(Js.Nullable.null);

  let getTopScroll = () => int_of_float(Utils.scrollTop) < 10 ? 75 : 20;

  let makeStyle = ind =>
    ReactDOMRe.Style.make(
      ~top=
        ind === 0
          ? string_of_int(getTopScroll()) ++ "px"
          : string_of_int(ind * 65 + getTopScroll()) ++ "px",
      (),
    );

  <div ref={ReactDOMRe.Ref.domRef(containerRef)}>
    {notifications->Belt.Array.mapWithIndex((ind, item) =>
       <Notification
         key={string_of_int(item.id)}
         id={item.id}
         type_={item.data.type_}
         style={makeStyle(ind)}>
         {ReasonReact.string(item.data.text)}
       </Notification>
     )
     |> ReasonReact.array}
  </div>;
};
