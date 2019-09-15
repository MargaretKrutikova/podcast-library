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

module Title = {
  [@react.component]
  let make = (~children) => {
    <MaterialUi_Typography
      gutterBottom=true
      variant=`H6
      className=Css.(style([lineHeight(`abs(1.3))]))>
      children
    </MaterialUi_Typography>;
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

module PodcastFilterButton = {
  [@react.component]
  let make = (~onDelete) => {
    <MaterialUi_Fab
      color=`Default
      size=`Small
      className=Css.(
        style([
          position(absolute),
          right(px(10)),
          top(px(10)),
          boxShadow(transparent),
        ])
      )
      onClick={_ => onDelete()}>
      <ReactFeather.SearchIcon />
    </MaterialUi_Fab>;
  };
};
