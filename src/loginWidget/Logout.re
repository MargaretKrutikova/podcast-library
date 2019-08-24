open UserTypes;

[@react.component]
let make = () => {
  let identity = ReactNetlifyIdentity.useIdentityContext();
  let fullName =
    identity.user
    ->Belt.Option.flatMap(user => user.metaData)
    ->Belt.Option.map(data => data##full_name);

  let handleLogout = _ => {
    identity.logoutUser();
    ignore();
  };
  <>
    {fullName->Belt.Option.mapWithDefault(React.null, name =>
       <div>
         <MaterialUi_Typography variant=`H5 component={`String("span")}>
           {React.string("Logged in as")}
         </MaterialUi_Typography>
         <MaterialUi_Typography> {React.string(name)} </MaterialUi_Typography>
       </div>
     )}
    <MaterialUi_Button
      onClick=handleLogout color=`Primary variant=`Contained fullWidth=true>
      {React.string("Log out")}
    </MaterialUi_Button>
  </>;
};
