type activeView =
  | Login
  | Signup
  | ForgotPassword;

let str = ReasonReact.string;

module Styles = {
  open Css;

  let dialog = theme =>
    style([
      width(vw(90.0)),
      margin2(~v=vh(10.0), ~h=`auto),
      maxWidth(px(400)) |> important,
      padding(px(theme |> Utils.spacingPx(2))),
      paddingTop(px(theme |> Utils.spacingPx(4))),
      media(Utils.getBreakpoint(`MD, theme), [width(vw(50.0))]),
    ]);

  let tab = theme => style([minHeight(px(Utils.spacingPx(1, theme) * 3))]);

  let title = style([paddingLeft(px(0))]);

  let tabRoot = theme =>
    style([
      marginBottom(px(theme |> Utils.spacingPx(2))),
      minHeight(px(Utils.spacingPx(1, theme) * 3)),
    ]);

  let closeButton = theme =>
    style([
      position(`absolute),
      right(px(theme |> Utils.spacingPx(1))),
      top(px(theme |> Utils.spacingPx(1))),
    ]);
};

[@react.component]
let make = (~open_, ~onLogin, ~onClose) => {
  let theme = Mui_Theme.useTheme();
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
    classes=[PaperScrollBody(Styles.dialog(theme))]>
    <MaterialUi_IconButton
      className={Styles.closeButton(theme)}
      color=`Inherit
      onClick={_ => onClose()}>
      <ReactFeather.CloseIcon />
    </MaterialUi_IconButton>
    <MaterialUi_DialogContent>
      {switch (activeView) {
       | ForgotPassword =>
         <>
           <MaterialUi_DialogTitle className=Styles.title>
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
             classes=[Root(Styles.tabRoot(theme))]
             variant=`FullWidth>
             <MaterialUi_Tab
               className={Styles.tab(theme)}
               onClick={_ => setActiveView(_ => Login)}
               label={React.string("Log in")}
             />
             <MaterialUi_Tab
               className={Styles.tab(theme)}
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
