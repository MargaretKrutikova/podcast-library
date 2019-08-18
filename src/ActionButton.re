open BsReactstrap;

type action =
  | Save
  | Remove;

[@react.component]
let make = (~disabled=false, ~onClick, ~action) => {
  switch (action) {
  | Save =>
    <Button size="sm" color="primary" disabled onClick>
      {React.string("Save")}
    </Button>
  | Remove =>
    <Button size="sm" color="warning" disabled onClick>
      {React.string("Remove")}
    </Button>
  };
};
