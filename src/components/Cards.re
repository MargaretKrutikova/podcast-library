let str = React.string;

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
  let make = (~children, ~gutterBottom=false) => {
    <MaterialUi_Typography
      gutterBottom
      variant=`H6
      className=Css.(style([lineHeight(`abs(1.3))]))>
      children
    </MaterialUi_Typography>;
  };
};
module Publisher = {
  [@react.component]
  let make = (~publisher, ~className="") => {
    <MaterialUi_Typography
      variant=`Subtitle1
      className={Css.(style([fontWeight(`num(500))])) ++ " " ++ className}
      color=`TextSecondary>
      {React.string("By " ++ publisher)}
    </MaterialUi_Typography>;
  };
};

module PodcastTitle = {
  [@react.component]
  let make = (~title) => {
    <MaterialUi_Typography
      variant=`Subtitle1 className=Css.(style([fontWeight(`num(500))]))>
      {React.string(title)}
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
  let make = (~onFilter) => {
    <MaterialUi_Button
      color=`Secondary
      size=`Small
      className=Css.(
        style([
          alignSelf(`flexStart),
          padding(px(0)),
          marginBottom(px(5)),
        ])
      )
      onClick={_ => onFilter()}>
      <ReactFeather.FilterIcon className=Css.(style([width(px(21))])) />
      <MaterialUi_Typography
        className=Css.(
          style([
            textTransform(`none),
            fontWeight(`num(600)),
            marginLeft(px(5)),
          ])
        )
        variant=`Body2
        component={`String("span")}>
        {React.string("Filter")}
      </MaterialUi_Typography>
    </MaterialUi_Button>;
  };
};

module CardMediaContainer = {
  [@react.component]
  let make = (~children) => {
    let theme = Mui_Theme.useTheme();

    <div
      className=Css.(
        style([
          display(`flex),
          alignItems(`center),
          marginBottom(px(theme |> Utils.spacingPx(2))),
          paddingTop(px(theme |> Utils.spacingPx(1))),
          media(Utils.getBreakpoint(`MD, theme), [marginBottom(px(0))]),
        ])
      )>
      children
    </div>;
  };
};

module PodcastCardContent = {
  [@react.component]
  let make = (~title, ~image, ~description, ~onFilter=?, ~info) => {
    let isDesktop = MediaHooks.useIsDesktop();

    <MaterialUi_CardContent>
      <Title> {str(title)} </Title>
      {isDesktop ? info : React.null}
      <CardMediaContainer>
        <CardMedia image />
        <div
          className=Css.(
            style([
              display(`flex),
              flexDirection(column),
              justifyContent(`center),
            ])
          )>
          {!isDesktop ? info : React.null}
          {onFilter->Belt.Option.mapWithDefault(React.null, filter =>
             <PodcastFilterButton onFilter=filter />
           )}
          {isDesktop ? <Description description /> : React.null}
        </div>
      </CardMediaContainer>
      {!isDesktop ? <Description description /> : React.null}
    </MaterialUi_CardContent>;
  };
};

module EpisodeCardContent = {
  [@react.component]
  let make =
      (
        ~title,
        ~image,
        ~description,
        ~lengthSec,
        ~pubDate,
        ~podcastTitle,
        ~publisher,
      ) => {
    let isDesktop = MediaHooks.useIsDesktop();
    let info = <Publisher publisher />;

    <MaterialUi_CardContent>
      <Title> {str(title)} </Title>
      {isDesktop ? info : React.null}
      <CardMediaContainer>
        <CardMedia image />
        <div
          className=Css.(
            style([
              display(`flex),
              flexDirection(column),
              justifyContent(`center),
            ])
          )>
          <PodcastTitle title=podcastTitle />
          <MaterialUi_Typography gutterBottom=true variant=`Subtitle2>
            {str(
               string_of_int(lengthSec / 60)
               ++ " mins, published "
               ++ Utils.formatDate(pubDate),
             )}
          </MaterialUi_Typography>
          {isDesktop ? <Description description /> : React.null}
        </div>
      </CardMediaContainer>
      {!isDesktop ? <Description description /> : React.null}
    </MaterialUi_CardContent>;
  };
};
