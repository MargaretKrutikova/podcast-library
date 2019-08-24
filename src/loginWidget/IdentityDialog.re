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

let str = ReasonReact.string;

[@react.component]
let make = (~open_, ~onClose: unit => unit) => {
  let classes = LoginDialogStyles.useStyles();
  let (activeTab, setActiveTab) = React.useState(() => 0);
  let identity = ReactNetlifyIdentity.useIdentityContext();

  <MaterialUi_Dialog
    open_
    onClose={(_, _) => onClose()}
    scroll=`Body
    classes=[PaperScrollBody(classes.root)]>
    <MaterialUi_DialogContent>
      {identity.isLoggedIn
         ? <Logout />
         : <>
             <MaterialUi_Tabs
               value=activeTab
               indicatorColor=`Primary
               textColor=`Primary
               classes=[Root(classes.tabRoot)]
               variant=`FullWidth>
               <MaterialUi_Tab
                 onClick={_ => setActiveTab(_ => 0)}
                 className={classes.tab}
                 label={React.string("Log in")}
               />
               <MaterialUi_Tab
                 label={React.string("Sign up")}
                 onClick={_ => setActiveTab(_ => 1)}
                 className={classes.tab}
               />
             </MaterialUi_Tabs>
             {activeTab === 0 ? <Login /> : <Signup />}
           </>}
      {identity.settings.providers->Belt.Array.length === 0
         ? React.null : <Providers />}
    </MaterialUi_DialogContent>
  </MaterialUi_Dialog>;
};
