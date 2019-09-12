[@react.component]
let make = (~children) => {
  let theme = Mui_Theme.useTheme();

  <div
    className=Css.(
      style([
        display(`grid),
        gridGap(px(theme |> Utils.spacingPx(2))),
        marginBottom(px(theme |> Utils.spacingPx(2))),
        marginTop(px(theme |> Utils.spacingPx(2))),
      ])
    )>
    children
  </div>;
};
