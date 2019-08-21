let shouldPreventDefault = e =>
  !ReactEvent.Mouse.metaKey(e) && !ReactEvent.Mouse.ctrlKey(e);

[@react.component]
let make =
    (
      ~href,
      ~className: string=?,
      ~variant=?,
      ~underline=?,
      ~component=?,
      ~children,
      ~color=?,
    ) => {
  let handleClick = event =>
    if (!ReactEvent.Mouse.defaultPrevented(event)
        && shouldPreventDefault(event)) {
      ReactEvent.Mouse.preventDefault(event);
      ReasonReact.Router.push(href);
    };
  <MuiLink_Extended
    href className onClick=handleClick ?color ?underline ?variant ?component>
    children
  </MuiLink_Extended>;
};
