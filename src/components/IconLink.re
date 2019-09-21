module Styles = {
  open Css;

  let btn = style([minWidth(`auto)]);

  let hideMobile = theme =>
    style([
      display(`none),
      media(Utils.getBreakpoint(`MD, theme), [display(`inlineFlex)]),
    ]);

  let iconBtn = theme =>
    style([
      media(
        Utils.getBreakpoint(`MD, theme),
        [marginRight(px(theme |> Utils.spacingPx(1)))],
      ),
    ]);
};

[@react.component]
let make = (~className="", ~route, ~text, ~children) => {
  let theme = Mui_Theme.useTheme();

  <RouterLink className href={Routing.getUrlFromRoute(route)}>
    <MaterialUi_Button
      className=Styles.btn color=`Inherit component={`String("span")}>
      children
      <span className={Styles.hideMobile(theme)}>
        {React.string(text)}
      </span>
    </MaterialUi_Button>
  </RouterLink>;
};
