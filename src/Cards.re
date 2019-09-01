module Styles = {
  open Css;

  let cardActions = theme =>
    style([
      padding(px(theme |> Utils.spacingPx(2))),
      paddingTop(px(0)),
      justifyContent(`flexEnd),
    ]);
  let searchCard = style([position(`relative), overflow(`visible)]);

  let cardSavedIcon = theme =>
    style([
      position(`absolute),
      right(px(theme |> Utils.spacingPx(1))),
      top(px(theme |> Utils.spacingPx(1) |> ( * )(-1))),
    ]);
  let cardDescription = style([overflow(`hidden)]);
};

module CardActions = {
  [@react.component]
  let make = (~children) => {
    let theme = Mui_Theme.useTheme();

    <MaterialUi_CardActions className={Styles.cardActions(theme)}>
      children
    </MaterialUi_CardActions>;
  };
};

module SearchCard = {
  [@react.component]
  let make = (~isSaved, ~children) => {
    let theme = Mui_Theme.useTheme();

    <MaterialUi_Card className=Styles.searchCard>
      {isSaved
         ? <ReactFeather.BookmarkIcon
             className={Styles.cardSavedIcon(theme)}
             fill="#669BBC"
             color="#29335C"
           />
         : React.null}
      children
    </MaterialUi_Card>;
  };
};

module Description = {
  [@react.component]
  let make = (~description) => {
    <MaterialUi_Typography
      variant=`Body1
      color=`TextSecondary
      component={`String("div")}
      className=Styles.cardDescription>
      <div
        dangerouslySetInnerHTML={
          "__html": Utils.truncateDescription(description),
        }
      />
    </MaterialUi_Typography>;
  };
};
