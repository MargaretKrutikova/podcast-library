open UserTypes.FormData;
external promiseErrorToJsObj: Js.Promise.error => Js.t('a) = "%identity";

let style = ReactDOMRe.Style.make;

[%mui.withStyles
  "LoginDialogStyles"(theme =>
    {
      forgotPasswordLink:
        style(
          ~textDecoration="underline",
          ~margin="0 auto",
          ~display="block",
          (),
        ),
      loginButton: style(~marginTop=theme |> Utils.spacing(1), ()),
      formElement: style(~marginBottom=theme |> Utils.spacing(2), ()),
      errorMsg: style(~textAlign="center", ~fontSize="16px", ()),
    }
  )
];

let str = ReasonReact.string;

[@react.component]
let make = () => {
  let classes = LoginDialogStyles.useStyles();
  let (state, dispatch) = UseReducerSafe.useReducerSafe(reducer, initState);
  let identityContext = ReactNetlifyIdentity.useIdentityContext();

  let {email, password, status} = state;

  let handleLogin = () => {
    dispatch(SubmitRequest);
    identityContext.loginUser(~email, ~password, ~remember=true, ())
    |> Js.Promise.then_(_ => dispatch(SubmitSuccess) |> Js.Promise.resolve)
    |> Js.Promise.catch(error =>
         dispatch(
           SubmitError(
             promiseErrorToJsObj(error)##message
             |> Js.String.replace("invalid_grant:", ""),
           ),
         )
         |> Js.Promise.resolve
       )
    |> ignore;
  };

  <form
    onSubmit={e => {
      ReactEvent.Form.preventDefault(e);
      handleLogin();
    }}>
    <MaterialUi_FormControl
      fullWidth=true classes=[Root(classes.formElement)]>
      <MaterialUi_TextField
        autoFocus=true
        label={str("Email Address")}
        type_="email"
        fullWidth=true
        value=email
        required=true
        name="email"
        disabled={status === Submitting}
        onChange={e => {
          let value = Utils.getInputValue(e);
          dispatch(SetEmail(value));
        }}
      />
    </MaterialUi_FormControl>
    <MaterialUi_FormControl
      fullWidth=true classes=[Root(classes.formElement)]>
      <MaterialUi_TextField
        label={str("Password")}
        type_="password"
        name="password"
        fullWidth=true
        required=true
        disabled={status === Submitting}
        value=password
        onChange={e => {
          let value = Utils.getInputValue(e);
          dispatch(SetPassword(value));
        }}
      />
    </MaterialUi_FormControl>
    <MaterialUi_FormControl
      fullWidth=true
      className={classes.loginButton}
      classes=[Root(classes.formElement)]>
      <MaterialUi_Button
        color=`Primary
        disabled={status === Submitting}
        variant=`Contained
        type_="submit"
        fullWidth=true>
        {str("Log in")}
      </MaterialUi_Button>
    </MaterialUi_FormControl>
    {switch (status) {
     | Error(msg) =>
       <MaterialUi_FormControl
         fullWidth=true className={classes.formElement} error=true>
         <MaterialUi_FormHelperText className={classes.errorMsg}>
           {str(msg)}
         </MaterialUi_FormHelperText>
       </MaterialUi_FormControl>
     | _ => React.null
     }}
    <MaterialUi_Link
      color=`Primary
      component={`String("button")}
      className={classes.forgotPasswordLink}>
      {str("Forgot password")}
    </MaterialUi_Link>
  </form>;
};
