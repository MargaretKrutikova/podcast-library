open BsReactstrap;

let str = ReasonReact.string;

module LibraryCardSubtitle = {
  [@react.component]
  let make = (~children) => {
    <CardSubtitle className="library-card-subtitle"> children </CardSubtitle>;
  };
};

module BottomActions = {
  [@react.component]
  let make = (~children) => {
    <div className="library-card-bottom-actions"> children </div>;
  };
};

module TopActions = {
  [@react.component]
  let make = (~children) => {
    <div className="library-card-top-actions"> children </div>;
  };
};

module SearchCardActions = {
  [@react.component]
  let make = (~isSaved, ~isSaving, ~onSave, ~children) => {
    <BottomActions>
      children
      {!isSaved
         ? <Button size="sm" color="primary" disabled=isSaving onClick=onSave>
             {str("Save")}
           </Button>
         : ReasonReact.null}
    </BottomActions>;
  };
};

module LibraryCard = {
  [@react.component]
  let make = (~hasTopActions=false, ~children) => {
    let className =
      "library-card" ++ (hasTopActions ? " library-card--top-actions" : "");

    <Card className> children </Card>;
  };
};

module SearchCard = {
  [@react.component]
  let make = (~isSaved, ~children) => {
    <LibraryCard hasTopActions=isSaved>
      {isSaved
         ? <TopActions>
             <Badge color="info"> {str("Saved")} </Badge>
           </TopActions>
         : ReasonReact.null}
      children
    </LibraryCard>;
  };
};
