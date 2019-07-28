open BsReactstrap;

let str = ReasonReact.string;

module LibraryCardSubtitle = {
  [@react.component]
  let make = (~children) => {
    <CardSubtitle className="mb-3"> children </CardSubtitle>;
  };
};

module BottomActions = {
  [@react.component]
  let make = (~children) => {
    <div className="mt-2 d-flex justify-content-end align-items-center">
      children
    </div>;
  };
};

module TopActions = {
  [@react.component]
  let make = (~children) => {
    <div className="library-card-top-actions d-flex justify-content-end">
      children
    </div>;
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
    let className = "mt-3 mb-3 " ++ (hasTopActions ? " pt-3" : "");

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
