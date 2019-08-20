module Container = {
  [@react.component]
  let make = (~children) => {
    <MaterialUi_Grid direction=`Column container=true spacing=V2>
      children
    </MaterialUi_Grid>;
  };
};

module Item = {
  [@react.component]
  let make = (~children) => {
    <MaterialUi_Grid item=true xs=V12> children </MaterialUi_Grid>;
  };
};
