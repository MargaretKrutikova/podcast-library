type action =
  | Save
  | Remove;

[@react.component]
let make = (~disabled=false, ~onClick, ~action) => {
  switch (action) {
  | Save =>
    <MaterialUi_Button
      size=`Small variant=`Contained color=`Primary disabled onClick>
      {React.string("Save")}
    </MaterialUi_Button>
  | Remove =>
    <MaterialUi_Button
      size=`Small color=`Primary variant=`Outlined disabled onClick>
      {React.string("Remove")}
    </MaterialUi_Button>
  };
};
