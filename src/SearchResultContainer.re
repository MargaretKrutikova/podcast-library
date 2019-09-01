[@react.component]
let make =
    (
      ~result: ReasonApolloHooks.Query.result('a),
      ~renderData: 'a => React.element,
    ) => {
  switch (result) {
  | {loading: true, data: None} => <Spinner />
  | {data: Some(data)} => renderData(data)
  | {error: Some(error)} =>
    <div> {ReasonReact.string(error##message)} </div>
  | {error: None, data: None, loading: false} =>
    <MaterialUi_Typography>
      {React.string("Not asked")}
    </MaterialUi_Typography>
  };
};

module LoadMoreButton = {
  [@react.component]
  let make = (~nextOffset, ~total, ~isFetching, ~onClick) => {
    let theme = Mui_Theme.useTheme();
    let hasMore = nextOffset < total;

    hasMore
      ? <MaterialUi_Button
          className=Css.(
            style([
              marginTop(px(theme |> Utils.spacingPx(2))),
              display(`block),
              margin2(~v=zero, ~h=auto),
            ])
          )
          onClick
          disabled=isFetching
          color=`Secondary
          variant=`Contained>
          {React.string("Fetch more")}
        </MaterialUi_Button>
      : ReasonReact.null;
  };
};
