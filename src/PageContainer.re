[@react.component]
let make = (~children) => {
  let theme = Mui_Theme.useTheme();

  <div
    className=Css.(
      style([
        marginTop(px(theme |> Utils.spacingPx(4))),
        media(
          Utils.getBreakpoint(`MD, theme),
          [maxWidth(px(800)), marginLeft(auto), marginRight(auto)],
        ),
      ])
    )>
    children
  </div>;
};
