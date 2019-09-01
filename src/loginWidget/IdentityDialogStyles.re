module Styles = {
  open Css;

  let submitButton = theme =>
    style([marginTop(px(theme |> Utils.spacingPx(1)))]);

  let formElement = theme =>
    style([marginBottom(px(theme |> Utils.spacingPx(2)))]);

  let divider = style([flexGrow(1.0), flexShrink(1.0), flexBasis(`auto)]);

  let providerSection = theme =>
    style([
      marginTop(px(theme |> Utils.spacingPx(3))),
      marginBottom(px(theme |> Utils.spacingPx(3))),
      display(`flex),
      alignItems(`center),
    ]);
};
