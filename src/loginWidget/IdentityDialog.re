type activeView =
  | Login
  | Signup
  | ForgotPassword;

let str = ReasonReact.string;

[@react.component]
let make = (~open_, ~onLogin, ~onClose) => {
  let classes = IdentityDialogStyles.IdentityDialogStyles.useStyles();
  let (activeView, setActiveView) = React.useState(() => Login);
  let identity = UserIdentity.Context.useIdentityContext();

  React.useLayoutEffect1(
    () => {
      if (open_) {
        setActiveView(_ => Login);
      };
      None;
    },
    [|open_|],
  );

  let hasProviders = identity.settings.providers->Belt.Array.length > 0;

  <MaterialUi_Dialog
    open_
    onClose={(_, _) => onClose()}
    scroll=`Body
    classes=[PaperScrollBody(classes.identityDialog)]>
    <MaterialUi_IconButton
      className={classes.closeButton} color=`Inherit onClick={_ => onClose()}>
      <ReactFeather.CloseIcon />
    </MaterialUi_IconButton>
    <MaterialUi_DialogContent>
      {switch (activeView) {
       | ForgotPassword =>
         <>
           <MaterialUi_DialogTitle className={classes.title}>
             {str("Recover password")}
           </MaterialUi_DialogTitle>
           <ForgotPassword gotoLogin={_ => setActiveView(_ => Login)} />
         </>
       | other =>
         <>
           <MaterialUi_Tabs
             value=activeView
             indicatorColor=`Primary
             textColor=`Primary
             classes=[Root(classes.tabRoot)]
             variant=`FullWidth>
             <MaterialUi_Tab
               className={classes.tab}
               onClick={_ => setActiveView(_ => Login)}
               label={React.string("Log in")}
             />
             <MaterialUi_Tab
               className={classes.tab}
               label={React.string("Sign up")}
               onClick={_ => setActiveView(_ => Signup)}
             />
           </MaterialUi_Tabs>
           {other === Login
              ? <Login
                  onLogin
                  gotoForgotPassword={_ => setActiveView(_ => ForgotPassword)}
                />
              : <Signup />}
           {hasProviders ? <Providers /> : React.null}
         </>
       }}
    </MaterialUi_DialogContent>
  </MaterialUi_Dialog>;
};
