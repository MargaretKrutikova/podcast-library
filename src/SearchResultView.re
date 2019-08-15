open BsReactstrap;

[@react.component]
let make =
    (
      ~result: ReasonApolloHooks.Query.result('a),
      ~renderData: 'a => React.element,
    ) => {
  switch (result) {
  | {loading: true, data: None} => <div> {React.string("Loading")} </div>
  | {data: Some(data)} => renderData(data)
  | {error: Some(error)} =>
    <div> {ReasonReact.string(error##message)} </div>
  | {error: None, data: None, loading: false} =>
    <div> {React.string("Not asked")} </div>
  };
};

module LoadMoreButton = {
  [@react.component]
  let make = (~nextOffset, ~total, ~isFetching, ~onClick) => {
    let hasMore = nextOffset < total;

    hasMore
      ? <div className="load-more-button-container">
          <Button color="info" onClick disabled=isFetching>
            {ReasonReact.string("Fetch more")}
          </Button>
        </div>
      : ReasonReact.null;
  };
};
