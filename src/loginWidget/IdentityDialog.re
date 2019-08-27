let style = ReactDOMRe.Style.make;
let addUnsafe = ReactDOMRe.Style.unsafeAddProp;
let combine = ReactDOMRe.Style.combine;

[%mui.withStyles
  "LoginDialogStyles"(theme =>
    {
      root:
        style(
          ~width="90vw",
          ~margin="10vh auto",
          ~maxWidth="400px!important",
          ~padding=theme |> Utils.spacing(2),
          ~paddingTop=theme |> Utils.spacing(4),
          (),
        )
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=style(~width="50vw", ()),
          ),
      tab:
        style(
          ~minHeight=
            (theme->MaterialUi.Theme.Theme.spacingGet(1) * 3)
            ->Utils.px_of_int,
          (),
        ),
      title: style(~paddingLeft="0", ()),
      closeButton:
        style(
          ~position="absolute",
          ~right=theme |> Utils.spacing(1),
          ~top=theme |> Utils.spacing(1),
          (),
        ),
      tabRoot:
        style(
          ~marginBottom=theme |> Utils.spacing(2),
          ~minHeight=
            (theme->MaterialUi.Theme.Theme.spacingGet(1) * 3)
            ->Utils.px_of_int,
          (),
        ),
    }
  )
];

type activeView =
  | Login
  | Signup
  | ForgotPassword;

let str = ReasonReact.string;

[@react.component]
let make = (~open_, ~onLogin, ~onClose) => {
  let classes = LoginDialogStyles.useStyles();
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
    classes=[PaperScrollBody(classes.root)]>
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
