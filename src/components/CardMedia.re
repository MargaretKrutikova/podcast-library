[@react.component]
let make = (~image) => {
  let theme = Mui_Theme.useTheme();

  image !== ""
    ? <MaterialUi_CardMedia
        className=Css.(
          style([
            width(px(80)),
            height(px(80)),
            borderRadius(px(4)),
            flexShrink(0.0),
            marginRight(px(theme |> Utils.spacingPx(2))),
            boxShadow(
              ~x=px(0),
              ~y=px(0),
              ~blur=px(20),
              `rgba((0, 0, 0, 0.1)),
            ),
            media(Utils.getBreakpoint(`MD, theme), [ width(px(100)),
            height(px(100)),]),
          ])
        )
        image
      />
    : React.null;
};
