[@react.component]
let make =
    (
      ~result: ReasonApolloHooks.Query.result('a),
      ~renderData: 'a => React.element,
    ) => {
  switch (result) {
  | {loading: true, data: None} =>
    <MaterialUi_Typography> {React.string("Loading")} </MaterialUi_Typography>
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
    let classes = AppStyles.AppStyles.useStyles();
    let hasMore = nextOffset < total;

    hasMore
      ? <MaterialUi_Button
          className={classes.cardLoadMoreBtn}
          onClick
          disabled=isFetching
          color=`Secondary
          variant=`Contained>
          {React.string("Fetch more")}
        </MaterialUi_Button>
      : ReasonReact.null;
  };
};
