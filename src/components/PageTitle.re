[@react.component]
let make = (~title) => {
  <MaterialUi_Typography
    className=Css.(style([fontWeight(`num(100)), textAlign(center)]))
    variant=`H3
    component={`String("h1")}
    gutterBottom=true>
    {React.string(title)}
  </MaterialUi_Typography>;
};
